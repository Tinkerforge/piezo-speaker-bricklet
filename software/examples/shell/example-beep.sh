#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Piezo Speaker Bricklet

# Make 2 second beep with a frequency of 1kHz
tinkerforge call piezo-speaker-bricklet $uid beep 2000 1000
