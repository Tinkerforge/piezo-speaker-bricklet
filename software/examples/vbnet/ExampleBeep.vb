Imports System
Imports Tinkerforge

Module ExampleBeep
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Piezo Speaker Bricklet

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim ps As New BrickletPiezoSpeaker(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Make 2 second beep with a frequency of 1kHz
        ps.Beep(2000, 1000)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
