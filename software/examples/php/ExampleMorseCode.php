<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletPiezoSpeaker.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletPiezoSpeaker;

$host = 'localhost';
$port = 4223;
$uid = '6tU'; // Change to your UID

$ipcon = new IPConnection(); // Create IP connection
$ps = new BrickletPiezoSpeaker($uid, $ipcon); // Create device object

$ipcon->connect($host, $port); // Connect to brickd
// Don't use device before ipcon is connected

// Morse SOS with a frequency of 2kHz
$ps->morseCode('... --- ...', 2000);

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));

?>
