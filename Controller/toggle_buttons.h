#ifndef JDF_OSCCONT_TOGGLE_BUTTONS
#define JDF_OSCCONT_TOGGLE_BUTTONS

#include <Arduino.h>

#define TOTAL_CHECK_BUTTONS 0
#define TOTAL_RADIO_BUTTONS 4
#define TOTAL_BUTTONS 4

class Toggle_buttons {
public:
  Toggle_buttons(uint8_t[], uint8_t[]);
  void update();

  bool bActive[TOTAL_BUTTONS];
  bool bToggled[TOTAL_BUTTONS];
  uint8_t check_button_pins[TOTAL_CHECK_BUTTONS];
  uint8_t radio_button_pins[TOTAL_RADIO_BUTTONS];

private:
  bool bCur_pressed[TOTAL_BUTTONS];
  bool bPrev_pressed[TOTAL_BUTTONS];
};

#endif