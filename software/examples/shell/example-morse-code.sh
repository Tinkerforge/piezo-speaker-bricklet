#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Morse SOS with a frequency of 2kHz
tinkerforge call piezo-speaker-bricklet $uid morse-code "... --- ..." 2000
