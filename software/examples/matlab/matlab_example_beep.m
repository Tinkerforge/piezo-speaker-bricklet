function matlab_example_beep
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletPiezoSpeaker;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'mGW'; % Change to your UID
    
    ipcon = IPConnection(); % Create IP connection
    ps = BrickletPiezoSpeaker(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Make 2 second beep
    ps.beep(2000, 1000);

    input('\nPress any key to exit...\n', 's');
    ipcon.disconnect();
end
