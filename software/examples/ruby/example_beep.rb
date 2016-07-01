#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_piezo_speaker'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Piezo Speaker Bricklet

ipcon = IPConnection.new # Create IP connection
ps = BrickletPiezoSpeaker.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Make 2 second beep with a frequency of 1kHz
ps.beep 2000, 1000

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
