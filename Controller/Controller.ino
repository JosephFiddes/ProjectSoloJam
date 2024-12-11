#include "toggle_buttons.h"

#define TOTAL_CHECK_BUTTONS 0
#define TOTAL_RADIO_BUTTONS 4
#define TOTAL_TRACK_OUTPUTS 4

const uint8_t check_button_pins[TOTAL_CHECK_BUTTONS] = {};
const uint8_t radio_button_pins[TOTAL_RADIO_BUTTONS] = {5,4,3,2};
const uint8_t display_pins[TOTAL_TRACK_OUTPUTS] = {11,10,9,8};
const uint8_t track_numbers[TOTAL_TRACK_OUTPUTS] = {1,2,3,4};

Toggle_buttons track_arm_buttons = Toggle_buttons(check_button_pins, TOTAL_CHECK_BUTTONS,
  radio_button_pins, TOTAL_RADIO_BUTTONS, 
  track_numbers, display_pins, TOTAL_TRACK_OUTPUTS, 
  true, "b/track/", "/recarm");

void setup() {
  //uint8_t pins[TOTAL_BUTTONS] = {2,4,5,6};
  //Toggle_buttons track_arm_buttons = Toggle_buttons(pins);
  Serial.begin(9600);
}

void loop() {
  uint8_t track_num;
  track_arm_buttons.update();

  Serial.flush();
}
