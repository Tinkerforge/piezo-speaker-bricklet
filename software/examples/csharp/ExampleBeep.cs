using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "ABC"; // Change to your UID

	static void Main() 
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletPiezoSpeaker ps = new BrickletPiezoSpeaker(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Make 2 second beep with a frequency of 1kHz
		ps.Beep(2000, 1000);

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
	}
}
