#!/usr/bin/perl  

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletPiezoSpeaker;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'xyz'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $ps = Tinkerforge::BrickletPiezoSpeaker->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Morse SOS with a frequency of 2kHz
$ps->morse_code('... --- ...', 2000);

print "\nPress any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

