#ifndef JDF_OSCCONT_TOGGLE_BUTTONS
#define JDF_OSCCONT_TOGGLE_BUTTONS

#include <Arduino.h>

class Toggle_buttons {
public:
  Toggle_buttons(uint8_t total_buttons, 
    uint32_t first_index,
    bool bToggle_on_button_up = false
);
  bool update(bool button_values[]);

  // This function is public to allow messages to be sent from other sources.
  // Useful for when we turn recording off after changing armed track.
  inline void send_message(const uint32_t value);

  bool* bToggled;

  uint8_t total_buttons;

private:
  inline void update_button_states(bool button_values[]);
  inline bool button_toggled(const bool cur_pressed, const bool prev_pressed);
  inline bool send_to_outputs();

  bool* bCur_pressed;
  bool* bPrev_pressed;

  uint32_t first_index;
  bool bToggle_on_button_up;
};

#endif