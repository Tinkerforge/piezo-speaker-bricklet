#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletPiezoSpeaker;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Piezo Speaker Bricklet

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $ps = Tinkerforge::BrickletPiezoSpeaker->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Make 2 second beep with a frequency of 1kHz
$ps->beep(2000, 1000);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
