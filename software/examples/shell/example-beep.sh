#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# Make 2 second beep with a frequency of 1kHz
tinkerforge call piezo-speaker-bricklet $uid beep 2000 1000
