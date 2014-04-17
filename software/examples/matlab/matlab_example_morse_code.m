function matlab_example_morse_code()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletPiezoSpeaker;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'mGW'; % Change to your UID
    
    ipcon = IPConnection(); % Create IP connection
    ps = BrickletPiezoSpeaker(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Morse SOS with a frequency of 2kHz
    ps.morseCode('... --- ...', 2000);

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end
