#include "toggle_buttons.h"

// #define DEBUG

//uint32_t Toggle_buttons::number_of_toggle_buttons = 0;

Toggle_buttons::Toggle_buttons(uint8_t total_buttons, uint32_t first_index,
bool bToggle_on_button_up) 
{


  // Allocate memory.
  this->bCur_pressed = (bool*) calloc(total_buttons, sizeof(bool));
  this->bPrev_pressed = (bool*) calloc(total_buttons, sizeof(bool));
  this->bToggled = (bool*) calloc(total_buttons, sizeof(bool));
  
  this->total_buttons = total_buttons;
  this->first_index = first_index;
  this->bToggle_on_button_up = bToggle_on_button_up;
}

// Returns true if a message was sent.
bool Toggle_buttons::update(bool button_values[])
{
  update_button_states(button_values);
  return send_to_outputs();  
}

// Updates bCur_pressed, bPrev_pressed, and bToggled.
inline void Toggle_buttons::update_button_states(bool button_values[])
{
  uint8_t i = 0;
  // Check buttons (each button toggles).
  for (i = 0; i < total_buttons; i++) {
    bToggled[i] = false;

    // Button pulls input low.
    bCur_pressed[i] = !button_values[i];

    if (button_toggled(bCur_pressed[i], bPrev_pressed[i])) {
      bToggled[i] = true;
    }

    bPrev_pressed[i] = bCur_pressed[i];
    

  }
}

inline bool Toggle_buttons::button_toggled(const bool cur_pressed, const bool prev_pressed) 
{
  return 
    // If toggle on button up: activate when the button is no longer being pressed.
    // (button up event)
    (bToggle_on_button_up && !cur_pressed && prev_pressed)
    ||
    // Otherwise: activate when button starts to be pressed.
    // (button down event)
    (!bToggle_on_button_up && cur_pressed && !prev_pressed);
}

inline bool Toggle_buttons::send_to_outputs() 
{
  bool bMessage_sent = false;
  //Serial.print("u");
  for (uint8_t i=0; i < total_buttons; i++) {
    
    if (bToggled[i]) {
      // OSC expects 32-bit big-endian two’s complement integer
      // int32_t value = (bActive[i] || bOSC_toggle) ? 1 : 0;
      send_message(first_index + i);

      bMessage_sent = true;
    }
  }

  return bMessage_sent;
}

inline void Toggle_buttons::send_message(const uint32_t index) {
  
  //Serial.write((byte*) &index, 4);

  Serial.write('B');
  #ifdef DEBUG  
    Serial.print(index);
    Serial.println();
  #else
    Serial.write((byte*) &index, 4);
    Serial.flush();
  #endif
} 