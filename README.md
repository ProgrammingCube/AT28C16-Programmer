# AT28C16-Programmer
An arduino nano based AT28C16 programmer.

# Tindie Link!!!
[AT28C16 Arduino Nano Programmer](https://www.tindie.com/products/gindiamond/at28c16-eeprom-programmer/)

## Getting Started

To start, make sure you have the latest [Arduino IDE]() and [Python 3]() installed.

Next, make sure you have [pyserial]() installed. In order to install pyserial via pip, issue this command:

`$pip install pyserial`

## Using the Programmer

1. Slot in your arduino nano into the headers so that the USB connector is hanging off the back of the board. If you want to use this programmer to dump an eeprom or examine its conents, now would be a good time to program it with the bin loader before adding in the eeprom.

1. Ensure that the ZIF socket is in the open position (lever bar is UP).

1. Gently install the AT28C16 into the socket, with the notch facing the arduino nano. Be sure that the pins aren't pushing oddly on the metal fins of the socket.

1. Gently close the lever to secure the eeprom in place. It is not a good idea to keep the socket closed for an extended period of time.

1. Connect up the programmer to your computer. The default baud rate is 9600.

## Using the Software

Unless you are using premade 2K bin files, you are responsible for creating these on your own. A quick python script to add FF's to the end of your existing executables should be a snap.

For this version of the software, you MUST use .bin files _exactly_ 2048 bytes long.

Once you have your .bin file, ensure that your arduino nano has the latest version of the nano_programmer firmware on board. Then connect up and issue this command:

`programmer.py [COMPORT] [e/r/p] [\..\file.bin]`

Example (Windows):

`programmer.py COM3 p ADD.bin`

Example (Linux):

`programmer.py /dev/ttyUSB0 p ADD.bin`

The flags are explained below:

- `e`: Erases the AT28C16 (fills with FF's). Does not need a file parameter.
- `r`: Reads the AT28C16. Does not need a file parameter.
- `p`: Writes the specified .bin file to the AT28C16.
