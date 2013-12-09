/* piezo-speaker-bricklet
 * Copyright (C) 2013 Olaf Lüke <olaf@tinkerforge.com>
 *
 * piezo-speaker.c: Implementation of Piezo Speaker Bricklet message
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "piezo-speaker.h"

#include "brickletlib/bricklet_entry.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/com/i2c/i2c_eeprom/i2c_eeprom_common.h"
#include "bricklib/drivers/adc/adc.h"
#include "config.h"
#include "bricklib/utility/util_definitions.h"
#include "bricklib/utility/init.h"

#define MORSE_DIT_LENGTH   100
#define MORSE_DAH_LENGTH   (MORSE_DIT_LENGTH*3)
#define MORSE_SPACE_LENGTH MORSE_DIT_LENGTH

void invocation(const ComType com, const uint8_t *data) {
	switch(((MessageHeader*)data)->fid) {
		case FID_BEEP: {
			beep(com, (Beep*)data);
			break;
		}

		case FID_MORSE_CODE: {
			morse_code(com, (MorseCode*)data);
			break;
		}

		case FID_CALIBRATE: {
			calibrate(com, (Calibrate*)data);
			break;
		}

		default: {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			break;
		}
	}
}

uint8_t get_i2c_address(void) {
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		return I2C_ADDRESS_HIGH;
	} else {
		return I2C_ADDRESS_LOW;
	}
}

uint16_t frequency_to_frequency_value(uint16_t frequency) {
	uint16_t distance = 0xFFFF;
	uint16_t best_match = 0;
	for(uint8_t i = 0; i < CALIBRATION_LENGTH; i++) {
		uint16_t new_distance = ABS(BC->frequency_match[i]-frequency);
		if(new_distance < distance) {
			distance = new_distance;
			best_match = i;
		}
	}

	if(best_match == 0) {
		return 0;
	}

	if(best_match == 64) {
		return 512;
	}

	int8_t add = 0;
	if(frequency < BC->frequency_match[best_match]) {
		add = -7 + SCALE(frequency, BC->frequency_match[best_match-1], BC->frequency_match[best_match], 0, 7);
	} else {
		add = SCALE(frequency, BC->frequency_match[best_match], BC->frequency_match[best_match+1], 0, 7);
	}

	return best_match*8 + add;
}

void set_frequency(uint16_t frequency) {
	frequency = MIN(frequency, FREQUENCY_VALUE_SUM_MAX);
	uint16_t value1 = frequency;
	uint16_t value2 = 0;

	if(frequency > FREQUENCY_VALUE_MAX) {
		value1 = FREQUENCY_VALUE_MAX;
		value2 = frequency - FREQUENCY_VALUE_MAX;
	}

	const uint8_t command_bit1 = value1 & (1 << 8) ? 1 : 0;
	const uint8_t command_bit2 = value2 & (1 << 8) ? 1 : 0;

	uint8_t data[3];
	data[0] = value1 & 0xFF;
	data[1] = I2C_COMMAND_WIPER1 | I2C_ACTION_WRITE | command_bit2;

	data[2] = value2 & 0xFF;

	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		BA->bricklet_select(BS->port - 'a');
		BA->TWID_Write(BA->twid,
		               get_i2c_address(),
					   I2C_COMMAND_WIPER0 | I2C_ACTION_WRITE | command_bit1,
					   1,
					   data,
					   3,
					   (Async*)NULL);
		BA->bricklet_deselect(BS->port - 'a');
		BA->mutex_give(*BA->mutex_twi_bricklet);
	}
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

	PIN_ENABLE.type = PIO_OUTPUT_0;
	PIN_ENABLE.attribute = PIO_DEFAULT;
    BA->PIO_Configure(&PIN_ENABLE, 1);

    PIN_FEEDBACK.type = PIO_INPUT;
    PIN_FEEDBACK.attribute = PIO_DEFAULT;
    BA->PIO_Configure(&PIN_FEEDBACK, 1);

    BC->morse_pos = 60;
    BC->morse_duration = 0;
    BC->morse_buzz = false;
    BC->beep_duration = 0;
    BC->morse_length = 0;

    load_calibration();
}

void destructor(void) {
	PIN_ENABLE.type = PIO_INPUT;
	PIN_ENABLE.attribute = PIO_PULLUP;
    BA->PIO_Configure(&PIN_ENABLE, 1);

    PIN_FEEDBACK.type = PIO_INPUT;
    PIN_FEEDBACK.attribute = PIO_PULLUP;
    BA->PIO_Configure(&PIN_FEEDBACK, 1);
}

void beep(const ComType com, const Beep *data) {
	// Disable morse code beeping
	BC->morse_pos = MAX_MORSE_LENGTH;
    BC->morse_duration = 0;
    BC->morse_buzz = false;

    set_frequency(frequency_to_frequency_value(data->frequency));

	// Enable beep
	PIN_ENABLE.pio->PIO_SODR = PIN_ENABLE.mask;
	BC->beep_duration = data->duration;

	BA->com_return_setter(com, data);
}

void morse_code(const ComType com, const MorseCode *data) {
	// Disable beep
	BC->beep_duration = 0;

	// Enable morse code beeping
	BC->morse_pos = 0;
	BC->morse_duration = 0;
	BC->morse_buzz = false;

	uint8_t i = 0;
	for(; i < MAX_MORSE_LENGTH; i++) {
		if(data->morse[i] != '.' && data->morse[i] != '-' && data->morse[i] != ' ') {
			break;
		}

		BC->morse[i] = data->morse[i];
	}

	BC->morse_length = i+1;
	set_frequency(frequency_to_frequency_value(data->frequency));

	BA->com_return_setter(com, data);
}

void load_calibration(void) {
    BA->bricklet_select(BS->port - 'a');
    BA->i2c_eeprom_master_read(BA->twid->pTwi,
                               EEPROM_CALIBRATION_POSITION,
                               (char *)BC->frequency_match,
                               EEPROM_CALIBRATION_LENGTH*2);
    BA->bricklet_deselect(BS->port - 'a');
}

void save_calibration(void) {
	// Save calibration in 32 byte chunks
    BA->bricklet_select(BS->port - 'a');
    BA->i2c_eeprom_master_write(BA->twid->pTwi,
                                EEPROM_CALIBRATION_POSITION,
                                (char *)BC->frequency_match,
                                32);
    BA->i2c_eeprom_master_write(BA->twid->pTwi,
                                EEPROM_CALIBRATION_POSITION+32,
                                (char *)BC->frequency_match+32,
                                32);
    BA->i2c_eeprom_master_write(BA->twid->pTwi,
                                EEPROM_CALIBRATION_POSITION+64,
                                (char *)BC->frequency_match+64,
                                32);
    BA->i2c_eeprom_master_write(BA->twid->pTwi,
                                EEPROM_CALIBRATION_POSITION+96,
                                (char *)BC->frequency_match+96,
                                32);
    BA->i2c_eeprom_master_write(BA->twid->pTwi,
                                EEPROM_CALIBRATION_POSITION+128,
                                (char *)BC->frequency_match+128,
                                2);
    BA->bricklet_deselect(BS->port - 'a');
}

void calibrate(const ComType com, const Calibrate *data) {
	__disable_irq();
	PIN_ENABLE.pio->PIO_SODR = PIN_ENABLE.mask;

	uint32_t tick_sum = 0;
	uint16_t tick_last = 0;
	for(uint16_t freq = 0; freq <= FREQUENCY_VALUE_SUM_MAX; freq+=8) {
		set_frequency(freq);
		SLEEP_MS(1);
		while(!(PIN_FEEDBACK.pio->PIO_PDSR & PIN_FEEDBACK.mask)) {
			__NOP();
		}
		while(PIN_FEEDBACK.pio->PIO_PDSR & PIN_FEEDBACK.mask) {
			__NOP();
		}
		tick_last = SysTick->VAL;
		SLEEP_US(1);
		while(!(PIN_FEEDBACK.pio->PIO_PDSR & PIN_FEEDBACK.mask)) {
			uint16_t tick_next = SysTick->VAL;
			if(tick_last < tick_next) {
				tick_sum += FEEDBACK_TICK_MAX - tick_next + tick_last;
			} else {
				tick_sum += tick_last - tick_next;
			}
			tick_last = tick_next;
		}

		SLEEP_US(1);

		do {
			uint16_t tick_next = SysTick->VAL;
			if(tick_last < tick_next) {
				tick_sum += FEEDBACK_TICK_MAX - tick_next + tick_last;
			} else {
				tick_sum += tick_last - tick_next;
			}
			tick_last = tick_next;
		} while(PIN_FEEDBACK.pio->PIO_PDSR & PIN_FEEDBACK.mask);

		uint16_t real_freq = (BOARD_MCK+tick_sum/2)/tick_sum;
		BC->frequency_match[freq/8] = real_freq;

		tick_sum = 0;
	}

	PIN_ENABLE.pio->PIO_CODR = PIN_ENABLE.mask;
	save_calibration();
	__enable_irq();

	CalibrateReturn cr;
	cr.header         = data->header;
	cr.header.length  = sizeof(CalibrateReturn);
	cr.calibration    = true;

	BA->send_blocking_with_timeout(&cr, sizeof(CalibrateReturn), com);
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		if(BC->beep_finished) {
			BC->beep_finished = false;
			StandardMessage sm;
			BA->com_make_default_header(&sm, BS->uid, sizeof(StandardMessage), FID_BEEP_FINISHED);

			BA->send_blocking_with_timeout(&sm,
										   sizeof(StandardMessage),
										   *BA->com_current);
		}

		if(BC->morse_finished) {
			BC->morse_finished = false;
			StandardMessage sm;
			BA->com_make_default_header(&sm, BS->uid, sizeof(StandardMessage), FID_MORSE_CODE_FINISHED);

			BA->send_blocking_with_timeout(&sm,
										   sizeof(StandardMessage),
										   *BA->com_current);
		}
	}

	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(BC->beep_duration > 0) {
			BC->beep_duration--;

			if(BC->beep_duration == 0) {
				PIN_ENABLE.pio->PIO_CODR = PIN_ENABLE.mask;
				BC->beep_finished = true;
			}
		} else if(BC->morse_duration > 0) {
			BC->morse_duration--;
			if(BC->morse_buzz) {

				// Make pause after dit or dah finished
				if(BC->morse_duration == 0) {
					PIN_ENABLE.pio->PIO_CODR = PIN_ENABLE.mask;
					BC->morse_buzz = false;
					BC->morse_duration = MORSE_SPACE_LENGTH;
				}
			}
		}

		bool not_ready = true;

		while(BC->morse_pos < BC->morse_length &&
			  BC->morse_duration == 0 &&
			  not_ready) {
			switch(BC->morse[BC->morse_pos]) {
				case '.':
					PIN_ENABLE.pio->PIO_SODR = PIN_ENABLE.mask;
					BC->morse_buzz = true;
					BC->morse_duration = MORSE_DIT_LENGTH;
					not_ready = false;
					break;
				case '-':
					PIN_ENABLE.pio->PIO_SODR = PIN_ENABLE.mask;
					BC->morse_buzz = true;
					BC->morse_duration = MORSE_DAH_LENGTH;
					not_ready = false;
					break;
				case ' ':
					BC->morse_buzz = false;
					BC->morse_duration = MORSE_SPACE_LENGTH;
					not_ready = false;
					break;
			}
			BC->morse_pos++;
			if(BC->morse_pos == BC->morse_length) {
				PIN_ENABLE.pio->PIO_CODR = PIN_ENABLE.mask;
				BC->morse_finished = true;
			}
		}
	}
}
