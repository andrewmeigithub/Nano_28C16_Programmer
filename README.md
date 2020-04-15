# Nano 28C16 Programmer
Inspired by Ben Eater. https://github.com/beneater/eeprom-programmer#circuit

Thanks to Liam Ilan for helping me with git. https://github.com/liam-ilan

## What [Nano_28C16_Programmer.ino](https://github.com/andrewmeigithub/Nano_28C16_Programmer/blob/master/Nano_28C16_Programmer.ino) does:
The file [Nano_28C16_Programmer.ino](https://github.com/andrewmeigithub/Nano_28C16_Programmer/blob/master/Nano_28C16_Programmer.ino) programs a 28C16 EEPROM using an Arduino Nano. It reads the EEPROM after writing and prints the values you wrote onto the EEPROM onto the serial monitor. **This only prints the first 256 addresses**. To change the number of addresses printed, change the integer printAddress on line 77.

## Circuit Diagram:
<img src="https://github.com/andrewmeigithub/Nano_28C16_Programmer/blob/master/Nano_28C16_Programmer_Circuit_Diagram.png">

## How to Use:
The Arduino Nano must be connected to the computer to program the 28C16.

1. Change values in line 17 of file [Nano_28C16_Programmer.ino](https://github.com/andrewmeigithub/Nano_28C16_Programmer/blob/master/Nano_28C16_Programmer.ino)
 in data[] array. The address of the data is located respectively.
2. Upload.
3. Open serial monitor to see the contents of the first 256 addresses.

There is also an option to clear the entire 28C16 by uncommenting line 117.

## Changes in Ben Eater's code:
1. Implemented data polling.
2. Used digitalWriteFast.h library.
3. Pulse width of write enable is now within recommended time (100-1000ns) using digitalWriteFast() function.
4. Changed output enable pin of 28C16 to connect to A0 of Arduino Nano, instead of being shifted out from 74HC595.
