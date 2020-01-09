#include "esphome.h"
//#include "switch.h"
#include "TM1638lite.h"


//esphome::lcd_base::LCDDisplay

using namespace esphome;
//using namespace esphome::switch_;

class TM1638Component;

class TM1638Led : public BinaryOutput {
 public:
  int i = 0;
  TM1638Component *parent = nullptr;

  void write_state(bool state) override {
//    digitalWrite(5, state);
    
  }
};

class MyCustomBinarySensor : public BinarySensor {
 public:
  int i = 0;
  TM1638Component *parent = nullptr;
};

class TM1638Component : public PollingComponent {
 public:
  TM1638lite tm = TM1638lite(12, 14, 4); // arduino pin numbers strobe/clock/data
  std::vector<BinarySensor *> buttons;
  std::vector<BinaryOutput *> leds;

  TM1638Component() : PollingComponent(100) { 
  for (uint8_t position = 0; position < 8; position++) {
    buttons.push_back(new MyCustomBinarySensor);
    leds.push_back(new TM1638Led);
  }
  }

  void setup() override {
    ESP_LOGI("tm1651", "Creation");
    tm.reset();

    tm.displayText("Hell0 !!");
    tm.setLED(0, 1);
  }

  void update() override {
//  return;
    static uint8_t btns = 0;
    uint8_t buttons = tm.readButtons();
    if (btns != buttons) {
        doLEDs(buttons);
	ESP_LOGI("tm1651", " update   %d", buttons);
	btns = buttons;
    }
//	ESP_LOGI("tm1651", " u   %d", buttons);

    // This is the actual sensor reading logic.
    //float temperature = bmp.readTemperature();
    //temperature_sensor->publish_state(temperature);

  }
  
  void doLEDs(uint8_t value) {
  for (uint8_t position = 0; position < 8; position++) {
    tm.setLED(position, value & 1);
    buttons[position]->publish_state(value & 1);

    value = value >> 1;
  }
 }

};
