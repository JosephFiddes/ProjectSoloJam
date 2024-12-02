#include "toggle_buttons.h"

const uint8_t button_pins[TOTAL_BUTTONS] = {5,4,3,2};
const uint8_t display_pins[TOTAL_BUTTONS] = {11,10,9,8};
Toggle_buttons track_arm_buttons = Toggle_buttons(button_pins);

void setup() {
  //uint8_t pins[TOTAL_BUTTONS] = {2,4,5,6};
  //Toggle_buttons track_arm_buttons = Toggle_buttons(pins);
}

void loop() {
  
  track_arm_buttons.update();

  for (uint8_t i=0; i < TOTAL_BUTTONS; i++) {
    if (track_arm_buttons.bToggled[i]) {
      digitalWrite(display_pins[i], track_arm_buttons.bActive[i]);
    }
  }
}
