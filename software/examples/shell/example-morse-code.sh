#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# Morse SOS with a frequency of 2kHz
tinkerforge call piezo-speaker-bricklet $uid morse-code "--- ... ---" 2000
