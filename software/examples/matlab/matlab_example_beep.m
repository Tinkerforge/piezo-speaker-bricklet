function matlab_example_beep()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletPiezoSpeaker;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Piezo Speaker Bricklet

    ipcon = IPConnection(); % Create IP connection
    ps = handle(BrickletPiezoSpeaker(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Make 2 second beep with a frequency of 1kHz
    ps.beep(2000, 1000);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
