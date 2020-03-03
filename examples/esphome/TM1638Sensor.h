#include "esphome.h"
#include "TM1638lite.h"
#include <string.h>

using namespace esphome;

class TM1638Component;

class TM1638Led : public Switch, public Component {
 public:
  const int i;
  TM1638lite &tm;

  TM1638Led(TM1638lite &t, const int n, const std::string name): Switch(name), i(n), tm(t)
  {
  }

  void write_state(bool state) override {
    tm.setLED(i, state & 1);
    publish_state(state);
  }

};

class TM1638Button : public BinarySensor {
 public:
  int i = 0;
  TM1638Component *parent = nullptr;
};

class TM1638Component : public PollingComponent, public CustomAPIDevice {
 public:
  TM1638lite tm = TM1638lite(12, 14, 4); // arduino pin numbers strobe/clock/data
  std::vector<BinarySensor *> buttons;
  std::vector<Switch *> leds;
  std::vector<TM1638Button *> buttons_;
  std::vector<TM1638Led *> leds_;
  char txt[10];
  int show_text = 0;
  int scroll = 0;

  TM1638Component() : PollingComponent(100) { 
  for (uint8_t position = 0; position < 8; position++) {
    auto btn = new TM1638Button;
    buttons_.push_back(btn);
    buttons.push_back(btn);


    const char *n ="012345678";
    char name[] = "led ";
    name[3]=n[position];

    auto led = new TM1638Led(tm, position, name);
    App.register_switch(led);
    leds_.push_back(led);
    leds.push_back(led);
  }
   strncpy(txt, "Hello        ", 10);
  }

  void setup() override {
    ESP_LOGI("tm1651", "Creation");
    tm.reset();

    tm.displayText(txt);

    register_service(&TM1638Component::on_set_text, "display_text", {"text", "time"});

    update();
  }
  
  void on_set_text(std::string text, int time)
  {
     strncpy(txt, text.c_str(), std::min(text.size(), size_t(10)));
     tm.displayText(txt);
     show_text = time;
  }
  
  void do_scroll()
  {
	    if (show_text == 0) {
		    auto time = id(homeassistant_time).now();
		    time.strftime(txt, 10, "%T");
		    tm.displayText(txt);
	    }
	    else {
		--show_text;
	    }
    }

  void update() override {
    if (scroll < 0) {
	scroll = 10;
	do_scroll();
    }
    static uint8_t btns = 3;
    uint8_t buttons = tm.readButtons();
    if (btns != buttons) {
        doLEDs(buttons);
	ESP_LOGI("tm1651", " update   %d", buttons);
	btns = buttons;
    }
   --scroll;
  }
  
  void doLEDs(uint8_t value) {
  for (uint8_t position = 0; position < 8; position++) {
    tm.setLED(position, value & 1 || leds[position]->state);
    buttons_[position]->publish_state(value);
    value = value >> 1;
  }
 }

};
