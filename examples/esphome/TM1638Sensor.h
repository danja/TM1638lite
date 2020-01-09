#include "esphome.h"
//#include "switch.h"
#include "TM1638lite.h"


//esphome::lcd_base::LCDDisplay

using namespace esphome;
//using namespace esphome::switch_;



class TM1638Led : public BinaryOutput {
 public:

  void write_state(bool state) override {
//    digitalWrite(5, state);
  }
};

class MyCustomBinarySensor : public BinarySensor {
 public:
};

class TM1638Component : public PollingComponent {
 public:
  TM1638lite tm = TM1638lite(4, 7, 8);
  std::vector<BinaryOutput *> buttons;
  std::vector<BinarySensor *> leds;

  TM1638Component() : PollingComponent(5000) { }

  void setup() override {
    tm.reset();
  }

  void update() override {
    // This is the actual sensor reading logic.
    //float temperature = bmp.readTemperature();
    //temperature_sensor->publish_state(temperature);

  }
};
