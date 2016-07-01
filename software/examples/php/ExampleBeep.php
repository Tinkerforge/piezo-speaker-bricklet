<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletPiezoSpeaker.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletPiezoSpeaker;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Piezo Speaker Bricklet

$ipcon = new IPConnection(); // Create IP connection
$ps = new BrickletPiezoSpeaker(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Make 2 second beep with a frequency of 1kHz
$ps->beep(2000, 1000);

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
