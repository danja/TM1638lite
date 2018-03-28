#include <TM1638lite.h>

// I/O pins on the Arduino connected to strobe, clock, data
// (power should go to 3.3v and GND)
TM1638lite tm(4, 7, 8);

void setup() {

  tm.reset();

  tm.displayText("Eh");
  tm.setLED(0, 1);
  
  delay(2000);

  tm.displayASCII(6, 'u');
  tm.displayASCII(7, 'p');
  tm.setLED(7, 1);

  delay(2000);

  tm.displayHex(0, 8);
  tm.displayHex(1, 9);
  tm.displayHex(2, 10);
  tm.displayHex(3, 11);
  tm.displayHex(4, 12);
  tm.displayHex(5, 13);
  tm.displayHex(6, 14);
  tm.displayHex(7, 15);

  delay(2000);

  tm.displayText("buttons");
}

void loop() {
  uint8_t buttons = tm.readButtons();
  doLEDs(buttons);
}

// scans the individual bits of value
void doLEDs(uint8_t value) {
  for (uint8_t position = 0; position < 8; position++) {
    tm.setLED(position, value & 1);
    value = value >> 1;
  }
}
