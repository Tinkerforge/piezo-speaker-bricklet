/* piezo-speaker-bricklet
 * Copyright (C) 2013 Olaf Lüke <olaf@tinkerforge.com>
 * Copyright (C) 2014 Matthias Bolte <matthias@tinkerforge.com>
 *
 * config.h: Piezo Speaker Bricklet specific configuration
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

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib/drivers/board/sam3s/SAM3S.h"

#include "piezo-speaker.h"

#define BRICKLET_FIRMWARE_VERSION_MAJOR 2
#define BRICKLET_FIRMWARE_VERSION_MINOR 0
#define BRICKLET_FIRMWARE_VERSION_REVISION 2

#define BRICKLET_HARDWARE_VERSION_MAJOR 1
#define BRICKLET_HARDWARE_VERSION_MINOR 0
#define BRICKLET_HARDWARE_VERSION_REVISION 0

#define BRICKLET_DEVICE_IDENTIFIER 242

#define BOARD_MCK 64000000
#define INVOCATION_IN_BRICKLET_CODE

#define PIN_ENABLE   (BS->pin1_ad)
#define PIN_FEEDBACK (BS->pin4_io)
#define MAX_MORSE_LENGTH 60
#define CALIBRATION_LENGTH 65

typedef struct {
	uint32_t beep_duration;
	uint32_t morse_duration;
	char morse[MAX_MORSE_LENGTH];
	bool morse_buzz;
	uint8_t morse_pos;

	uint8_t morse_length;

	bool beep_finished;
	bool morse_finished;

	uint16_t frequency_match[CALIBRATION_LENGTH];
} BrickContext;

#endif
