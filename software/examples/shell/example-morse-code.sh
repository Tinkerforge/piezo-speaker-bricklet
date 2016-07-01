#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Piezo Speaker Bricklet

# Morse SOS with a frequency of 2kHz
tinkerforge call piezo-speaker-bricklet $uid morse-code "... --- ..." 2000
