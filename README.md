# TM1638lite
Small(ish) interface library from Arduino to TM1638 display chip/card

Install by copying the TM1638 directory under your Arduino libraries folder.

For use, see examples/demo.

Here's what I've been doing with it : [DOG-1](https://github.com/danja/dog)

![pic of cards](https://github.com/danja/TM1638lite/blob/master/pic.jpg?raw=true)

The TM1638's power should go to 3.3v and ground on the Arduino. Its strobe, clock and data pins should be connected to data I/O pins on the Arduino (a good choice is 4, 7, 8). These values should be sent to the constructor of the TM1638 class.

## From TM1638.h :

**TM1638lite (uint8_t strobe, uint8_t clock, uint8_t data)**

**void 	reset ()**
 	clear the TM1638 - use in setup()

**uint8_t 	readButtons ()**
 	bits in returned value correspond to state of buttons

**void 	setLED (uint8_t position, uint8_t value)**
 	turns LED at position on (value=1) or off (value=0)

**void 	displayText (String text)**
 	displays an approximation of the text on the 7-segment display

**void 	displaySS (uint8_t position, uint8_t value)**
 	direct access to 7-segment display at position, bits of value state which segments to turn on

**void 	displayASCII (uint8_t position, uint8_t ascii)**
 	displays an approximation of the ASCII character at position

**void 	displayHex (uint8_t position, uint8_t hex)**
 	displays a hex representation of the value at position

**void 	sendCommand (uint8_t value)**
   	send low-level command, probably won't be needed
