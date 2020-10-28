# AT28C16-Programmer
An arduino nano based AT28C16 programmer.


## Getting Started

To start, make sure you have the latest [Arduino IDE]() and [Python 3]() installed.

Next, make sure you have [pyserial]() installed. In order to install pyserial via pip, issue this command:

`$pip install pyserial`

## Using the Programmer

1. Slot in your arduino nano into the headers so that the USB connector is hanging off the back of the board. If you want to use this programmer to dump an eeprom or examine its conents, now would be a good time to program it with the bin loader before adding in the eeprom.

1. Ensure that the ZIF socket is in the open position (lever bar is UP).

1. Gently install the AT28C16 into the socket, with the notch facing the arduino nano. Be sure that the pins aren't pushing oddly on the metal fins of the socket.

1. Gently close the lever to secure the eeprom in place. It is not a good idea to keep the socket closed for an extended period of time.