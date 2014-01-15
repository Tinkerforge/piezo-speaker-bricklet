<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletPiezoSpeaker.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletPiezoSpeaker;

const HOST = 'localhost';
const PORT = 4223;
const UID = '6tU'; // Change to your UID

$ipcon = new IPConnection(); // Create IP connection
$ps = new BrickletPiezoSpeaker(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Morse SOS with a frequency of 2kHz
$ps->morseCode('... --- ...', 2000);

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));

?>
