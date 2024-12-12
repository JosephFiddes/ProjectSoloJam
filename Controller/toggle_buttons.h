#ifndef JDF_OSCCONT_TOGGLE_BUTTONS
#define JDF_OSCCONT_TOGGLE_BUTTONS

#include <Arduino.h>

class Toggle_buttons {
public:
  Toggle_buttons(uint8_t[], uint8_t, uint8_t[], uint8_t,
  uint8_t[], uint8_t[], uint8_t, 
  bool, const char*, const char*, bool bOSC_toggle = false);
  void update();

  uint8_t* check_button_pins;
  uint8_t* radio_button_pins;
  bool* bActive;
  bool* bToggled;
  uint8_t* output_tracks;
  uint8_t* output_pins;

  uint8_t total_check_buttons;
  uint8_t total_radio_buttons;
  uint8_t total_outputs;

private:
  inline void update_check_buttons();
  inline void update_radio_buttons();
  inline void send_to_outputs();

  bool* bCur_pressed_check;
  bool* bPrev_pressed_check;
  bool* bCur_pressed_radio;
  bool* bPrev_pressed_radio;

  bool bOutput_track_in_message;
  uint8_t message_length;
  char* message_part1;
  char* message_part2;

  bool bOSC_toggle;
};

#endif