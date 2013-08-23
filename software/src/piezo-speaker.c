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
#include "bricklib/drivers/adc/adc.h"
#include "config.h"
#include "bricklib/utility/util_definitions.h"
#include "bricklib/utility/init.h"

#define MORSE_DIT_LENGTH 100
#define MORSE_DAH_LENGTH MORSE_DIT_LENGTH*3
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

void set_frequency(uint16_t frequency) {
	frequency = BETWEEN(460, frequency, 7000);
	const uint16_t value = SCALE(frequency, 460, 7000, 0, 256);

	const uint8_t command_bit = value & (1 << 8) ? 1 : 0;

	uint8_t data[3];
	data[0] = value & 0xFF;
	data[1] = I2C_COMMAND_WIPER1 | I2C_ACTION_WRITE | command_bit;
	data[2] = data[0];

	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		BA->bricklet_select(BS->port - 'a');
		BA->TWID_Write(BA->twid,
		               get_i2c_address(),
					   I2C_COMMAND_WIPER0 | I2C_ACTION_WRITE | command_bit,
					   1,
					   &data,
					   3,
					   (Async*)NULL);
		BA->bricklet_deselect(BS->port - 'a');
		BA->mutex_give(*BA->mutex_twi_bricklet);
	}
}

void constructor(void) {
	PIN_ENABLE.type = PIO_OUTPUT_0;
	PIN_ENABLE.attribute = PIO_DEFAULT;
    BA->PIO_Configure(&PIN_ENABLE, 1);

    BC->morse_pos = 60;
    BC->morse_duration = 0;
    BC->morse_buzz = false;
    BC->beep_duration = 0;
}

void destructor(void) {
	PIN_ENABLE.type = PIO_INPUT;
	PIN_ENABLE.attribute = PIO_PULLUP;
    BA->PIO_Configure(&PIN_ENABLE, 1);
}

void beep(const ComType com, const Beep *data) {
	// Disable morse code beeping
	BC->morse_pos = MORSE_LENGTH;
    BC->morse_duration = 0;
    BC->morse_buzz = false;

    set_frequency(data->frequency);

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

	for(uint8_t i = 0; i < MORSE_LENGTH; i++) {
		if(data->morse[i] != '.' && data->morse[i] != '-' && data->morse[i] != ' ') {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
			return;
		}

		BC->morse[i] = data->morse[i];
	}

	set_frequency(data->frequency);

	BA->com_return_setter(com, data);
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

		while(BC->morse_pos < MORSE_LENGTH &&
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
			if(BC->morse_pos == MORSE_LENGTH) {
				PIN_ENABLE.pio->PIO_CODR = PIN_ENABLE.mask;
				BC->morse_finished = true;
			}
		}
	}
}
