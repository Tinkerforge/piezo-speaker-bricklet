var IPConnection = require('Tinkerforge/IPConnection');
var BrickletPiezoSpeaker = require('Tinkerforge/BrickletPiezoSpeaker');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'iMv';// Change to your UID

var ipcon = new IPConnection();// Create IP connection
var ps = new BrickletPiezoSpeaker(UID, ipcon);// Create device object

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);        
    }
);// Connect to brickd

// Don't use device before ipcon is connected
ipcon.on(IPConnection.CALLBACK_CONNECTED,
    function(connectReason) {
        //Make 2 second beep with a frequency of 1kHz
        ps.beep(2000, 1000);      
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

