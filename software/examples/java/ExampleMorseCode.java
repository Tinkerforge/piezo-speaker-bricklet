import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletPiezoSpeaker;

public class ExampleMorseCode {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletPiezoSpeaker ps = new BrickletPiezoSpeaker(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Morse SOS with a frequency of 2kHz
		ps.morseCode("... --- ...", 2000);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
