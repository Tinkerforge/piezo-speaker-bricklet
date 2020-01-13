#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Piezo Speaker Bricklet

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_piezo_speaker import BrickletPiezoSpeaker

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    ps = BrickletPiezoSpeaker(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Morse SOS with a frequency of 2kHz
    ps.morse_code("... --- ...", 2000)

    input("Press key to exit\n") # Use raw_input() in Python 2
    ipcon.disconnect()
