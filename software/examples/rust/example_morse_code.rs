use std::{error::Error, io};

use tinkerforge::{ipconnection::IpConnection, piezo_speaker_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Piezo Speaker Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let piezo_speaker_bricklet = PiezoSpeakerBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Morse SOS with a frequency of 2kHz
    piezo_speaker_bricklet.morse_code("... --- ...".to_string(), 2000);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
