#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_piezo_speaker'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change to your UID

ipcon = IPConnection.new # Create IP connection
ps = BrickletPiezoSpeaker.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Morse SOS with a frequency of 2kHz
ps.morse_code '... --- ...', 2000

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
