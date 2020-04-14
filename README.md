# Nano 28C16 Programmer
Inspired by Ben Eater https://github.com/beneater/eeprom-programmer#circuit.
Thanks to Liam Ilan for helping me with git.

## What Nano_28C16_Programmer.ino does:
The file Nano_28C16_Programmer.ino programs a 28C16 EEPROM using an Arduino Nano. It reads the EEPROM after writing and prints the values you wrote onto the EEPROM onto the serial monitor. **This only prints the first 256 addresses**. To change the number of addresses printed, change the integer printAddress on line 77.

## Circuit Diagram:
![](https://placekitten.com/200/300)

## How to Use:
1. Change values in line 17 of file Nano_28C16_Programmer.ino in data[] array. The address of the data is located respectively.
2. Upload.

There is also an option to clear the entire 28C16 by uncommenting line 17.

## Changes in Ben Eater's code:
1. Implemented data polling.
2. Used digitalWriteFast.h library.
3. Pulse width of write enable is now within recommended time (100-1000ns) using digitalWriteFast() function.
4. Changed output enable pin to A0 of Arduino Nano instead of shifted out from 74HC595.
