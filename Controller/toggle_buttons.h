#ifndef JDF_OSCCONT_TOGGLE_BUTTONS
#define JDF_OSCCONT_TOGGLE_BUTTONS

#include <Arduino.h>

typedef union {
  int32_t ivalue;
  byte bits[4];
} Binary_int32;

class Toggle_buttons {
public:
  Toggle_buttons(uint8_t total_check_buttons, uint8_t total_radio_buttons,
    const char* message_part1, 
    bool bToggle_on_button_up = false,
    bool bOutput_track_in_message = false, 
    uint8_t track_numbers[] = {}, 
    uint8_t total_tracks = 1, 
    const char* message_part2 = "",
    bool bOSC_toggle = true);
  void update(bool check_buttons[], bool radio_buttons[]);

  bool* bActive;
  bool* bToggled;
  uint8_t* track_numbers;

  uint8_t total_check_buttons;
  uint8_t total_radio_buttons;
  uint8_t total_tracks;

private:
  inline void update_check_buttons(bool check_buttons[]);
  inline void update_radio_buttons(bool radio_buttons[]);
  inline bool button_toggled(const bool cur_pressed, const bool prev_pressed);
  inline void send_to_outputs();

  bool* bCur_pressed_check;
  bool* bPrev_pressed_check;
  bool* bCur_pressed_radio;
  bool* bPrev_pressed_radio;

  bool bToggle_on_button_up;
  bool bOutput_track_in_message;
  uint8_t message_length;
  char* message_part1;
  char* message_part2;

  bool bOSC_toggle;
};

#endif