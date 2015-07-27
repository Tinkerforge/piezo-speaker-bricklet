import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletPiezoSpeaker;

public class ExampleBeep {
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

		// Make 2 second beep with a frequency of 1kHz
		ps.beep(2000, 1000);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
