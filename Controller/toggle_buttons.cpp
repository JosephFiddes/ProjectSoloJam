#include "toggle_buttons.h"

//#define DEBUG

Toggle_buttons::Toggle_buttons(uint8_t total_check_buttons, uint8_t total_radio_buttons,
const char* message_part1, 
bool bOutput_track_in_message, uint8_t track_numbers[], uint8_t total_tracks, 
const char* message_part2,
bool bOSC_toggle) 
{
  // Allocate memory.
  this->bActive = (bool*) calloc(total_tracks, sizeof(bool));
  this->bToggled = (bool*) calloc(total_tracks, sizeof(bool));
  this->track_numbers = (uint8_t*) malloc(sizeof(uint8_t)*total_tracks);
  this->bCur_pressed_check = (bool*) malloc(sizeof(bool)*total_check_buttons);
  this->bPrev_pressed_check = (bool*) calloc(total_check_buttons, sizeof(bool));
  this->bCur_pressed_radio = (bool*) malloc(sizeof(bool)*total_radio_buttons);
  this->bPrev_pressed_radio = (bool*) calloc(total_radio_buttons, sizeof(bool));

  // Initialise check buttons (toggled when pressed)
  this->total_check_buttons = total_check_buttons;

  // Initialise radio buttons (activated when pressed, deactivates others)
  this->total_radio_buttons = total_radio_buttons;

  // Initialise outputs
  this->total_tracks = total_tracks;
  for (uint8_t i = 0; i < total_tracks; i++) {
    (this->track_numbers)[i] = track_numbers[i];
  }

  this->bOutput_track_in_message = bOutput_track_in_message;

  // Get length of OSC message for quicker parsing.
  uint8_t c1 = 0;
  uint8_t c2 = 0;
  for (c1=0; message_part1[c1] != '\0'; c1++);
  this->message_part1 = (char*) malloc(sizeof(char)*(c1+1));
  strcpy(this->message_part1, message_part1);

  if (bOutput_track_in_message) {
    for (c2=0; message_part2[c2] != '\0'; c2++);
    // Include null-terminator, but don't include length of track num, 
    // since it depends on track num value.
    this->message_length = c1 + c2 + 1; 

    this->message_part2 = (char*) malloc(sizeof(char)*(c2+1));
    strcpy(this->message_part2, message_part2);
  } else {
    this->message_length = c1 + 1; // Include null-terminator.
  }

  this->bOSC_toggle = bOSC_toggle;
}

void Toggle_buttons::update(bool check_buttons[], bool radio_buttons[])
{
  for (uint8_t i = 0; i < total_tracks; i++) {
    bToggled[i] = false;
  }

  update_check_buttons(check_buttons);
  update_radio_buttons(radio_buttons);
  send_to_outputs();  
}

inline void Toggle_buttons::update_check_buttons(bool check_buttons[])
{
  // Check buttons (each button toggles).
  for (uint8_t i = 0; i < total_check_buttons; i++) {
    bToggled[i] = false;

    // Button pulls input low.
    bCur_pressed_check[i] = !check_buttons[i];

    if (!bCur_pressed_check[i] && bPrev_pressed_check[i]) {
      bToggled[i] = true;
      bActive[i] = !bActive[i];
    }

    bPrev_pressed_check[i] = bCur_pressed_check[i];
  }
}

inline void Toggle_buttons::update_radio_buttons(bool radio_buttons[])
{
  // Radio button (pressing this button activates this button,
  // and deactivates all other buttons.)
  uint8_t i;
  bool bRadio_toggled = false;
  uint8_t new_active_button;

  // Find toggled radio button, if there is one.
  for (i = 0; i < total_radio_buttons; i++) {
    // Button pulls input low.
    bCur_pressed_radio[i] = !radio_buttons[i];

    // Button up event.
    if (!bCur_pressed_radio[i] && bPrev_pressed_radio[i]) {
      bRadio_toggled = true;
      new_active_button = i;
    }

    bPrev_pressed_radio[i] = bCur_pressed_radio[i];
  }

  // If any of the radio buttons were toggled, the controller needs to
  // go through and deactivate all the other buttons.
  if (bRadio_toggled) {
    for (i = 0; i < total_radio_buttons; i++) {
      if (i == new_active_button) {
        // The new active button.
        // Toggle if not already ON
        if (!bActive[i]) { 
          bActive[i] = true;
          bToggled[i] = true;
        } else {
          bToggled[i] = false;
        }
      } else {
        // The other buttons.
        // Toggle if not already OFF
        if (bActive[i]) { 
          bActive[i] = false;
          bToggled[i] = true;
        } else {
          bToggled[i] = false;
        }
      }
    }
  }
}

inline void Toggle_buttons::send_to_outputs() 
{
  Binary_int32 value_to_write;

  for (uint8_t i=0; i < total_tracks; i++) {
    if (bToggled[i]) {
      // OSC expects 32-bit big-endian two’s complement integer 
      value_to_write.ivalue = (bActive[i] || bOSC_toggle) ? 1 : 0;
      
      // Write data.
      if (bOutput_track_in_message) {
        // Add track number to message length.
        uint8_t true_message_length = (track_numbers[i] >= 10) ? 
          message_length + 2 : message_length + 1;

        Serial.write(true_message_length);
        Serial.write(message_part1);
        Serial.print(track_numbers[i]);
        Serial.write(message_part2);
        Serial.write('\0');
      } else {
        Serial.write(message_length);
        Serial.write(message_part1);
        Serial.write('\0');
      }
      
      Serial.write(value_to_write.bits, 4);
      Serial.write('i');
      Serial.write('\0');

    #ifdef DEBUG  
      Serial.println();
    #else
      Serial.flush();
    #endif
    }
  }
}

