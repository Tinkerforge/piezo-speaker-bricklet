package main

import (
	"fmt"
	"github.com/tinkerforge/go-api-bindings/ipconnection"
	"github.com/tinkerforge/go-api-bindings/piezo_speaker_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Piezo Speaker Bricklet.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	ps, _ := piezo_speaker_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Morse SOS with a frequency of 2kHz
	ps.MorseCode("... --- ...", 2000)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
