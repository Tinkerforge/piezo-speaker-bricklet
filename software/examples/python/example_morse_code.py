#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_piezo_speaker import PiezoSpeaker

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    ps = PiezoSpeaker(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Morse SOS with a frequency of 2kHz
    ps.morse_code('... --- ...', 2000)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
