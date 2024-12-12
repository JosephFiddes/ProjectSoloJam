#include "toggle_buttons.h"

#define TOTAL_RECARM_CHECK_BUTTONS 0
#define TOTAL_RECARM_RADIO_BUTTONS 3
#define TOTAL_RECARM_TRACK_OUTPUTS 3

const uint8_t recarm_check_button_pins[TOTAL_RECARM_CHECK_BUTTONS] = {};
const uint8_t recarm_radio_button_pins[TOTAL_RECARM_RADIO_BUTTONS] = {5,4,3}; // 2
const uint8_t recarm_display_pins[TOTAL_RECARM_TRACK_OUTPUTS] = {11,10,9}; // 8
const uint8_t track_numbers[TOTAL_RECARM_TRACK_OUTPUTS] = {1,2,3}; // 4

Toggle_buttons track_arm_buttons = Toggle_buttons(recarm_check_button_pins, TOTAL_RECARM_CHECK_BUTTONS,
  recarm_radio_button_pins, TOTAL_RECARM_RADIO_BUTTONS, 
  track_numbers, recarm_display_pins, TOTAL_RECARM_TRACK_OUTPUTS, 
  true, "b/track/", "/recarm");

#define TOTAL_RECORD_CHECK_BUTTONS 1
#define TOTAL_RECORD_RADIO_BUTTONS 0
#define TOTAL_RECORD_OUTPUTS 1


const uint8_t record_check_button_pins[TOTAL_RECORD_CHECK_BUTTONS] = {2};
const uint8_t record_radio_button_pins[TOTAL_RECORD_RADIO_BUTTONS] = {};
const uint8_t record_display_pins[TOTAL_RECORD_OUTPUTS] = {8};

Toggle_buttons record_button = Toggle_buttons(record_check_button_pins, TOTAL_RECORD_CHECK_BUTTONS,
  record_radio_button_pins, TOTAL_RECORD_RADIO_BUTTONS, 
  nullptr, record_display_pins, TOTAL_RECORD_OUTPUTS, 
  false, "t/record", "", true);

void setup() {
  //uint8_t pins[TOTAL_BUTTONS] = {2,4,5,6};
  //Toggle_buttons track_arm_buttons = Toggle_buttons(pins);
  Serial.begin(9600);
}

void loop() {
  track_arm_buttons.update();
  record_button.update();

  Serial.flush();
}
