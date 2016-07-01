function octave_example_morse_code()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Piezo Speaker Bricklet

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    ps = java_new("com.tinkerforge.BrickletPiezoSpeaker", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Morse SOS with a frequency of 2kHz
    ps.morseCode("... --- ...", 2000);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
