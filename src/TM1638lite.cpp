#include "Arduino.h"
#include "TM1638lite.h"

uint8_t STROBE_IO;
uint8_t DATA_IO;
uint8_t CLOCK_IO;

TM1638lite::TM1638lite(uint8_t strobe, uint8_t clock, uint8_t data) {
  STROBE_IO = strobe;
  DATA_IO = data;
  CLOCK_IO = clock;
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  sendCommand(ACTIVATE);
  reset();
}

void TM1638lite::sendCommand(uint8_t value)
{
  digitalWrite(STROBE_IO, LOW);
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, value);
  digitalWrite(STROBE_IO, HIGH);
}

void TM1638lite::reset() {
  sendCommand(WRITE_INC); // set auto increment mode
  digitalWrite(STROBE_IO, LOW);
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, 0xc0);   // set starting address to 0
  for (uint8_t i = 0; i < 16; i++)
  {
    shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, 0x00);
  }
  digitalWrite(STROBE_IO, HIGH);
}

uint8_t TM1638lite::readButtons()
{
  uint8_t buttons = 0;
  digitalWrite(STROBE_IO, LOW);
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, BUTTONS);
  pinMode(DATA_IO, INPUT);

  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftIn(DATA_IO, CLOCK_IO, LSBFIRST) << i;
    buttons |= v;
  }

  pinMode(DATA_IO, OUTPUT);
  digitalWrite(STROBE_IO, HIGH);
  return buttons;
}

void TM1638lite::setLED(uint8_t position, uint8_t value)
{
  pinMode(DATA_IO, OUTPUT);
  sendCommand(WRITE_LOC);
  digitalWrite(STROBE_IO, LOW);
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, 0xC1 + (position << 1));
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, value);
  digitalWrite(STROBE_IO, HIGH);
}

void TM1638lite::displayText(String text) {
  uint8_t length = text.length();

  for (uint8_t i = 0; i < length; i++) {
    for (uint8_t position = 0; position < 8; position++) {
      displayASCII(position, text[position]);
    }
  }
}

void TM1638lite::displaySS(uint8_t position, uint8_t value) { // call 7-segment
  sendCommand(WRITE_LOC);
  digitalWrite(STROBE_IO, LOW);
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, 0xC0 + (position << 1));
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, value);
  digitalWrite(STROBE_IO, HIGH);
}

void TM1638lite::displayASCII(uint8_t position, uint8_t ascii) {
  displaySS(position, ss[ascii]);
}

void TM1638lite::displayHex(uint8_t position, uint8_t hex) {
  displaySS(position, hexss[hex]);
}
