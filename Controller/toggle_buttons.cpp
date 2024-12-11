#include "toggle_buttons.h"

Toggle_buttons::Toggle_buttons(uint8_t check_button_pins[], uint8_t total_check_buttons,
uint8_t radio_button_pins[], uint8_t total_radio_buttons,
uint8_t output_tracks[], uint8_t output_pins[], uint8_t total_outputs,
bool bOutput_track_in_message, const char* message_part1, const char* message_part2) 
{
  // Allocate memory.
  this->bActive = (bool*) calloc(total_outputs, sizeof(bool));
  this->bToggled = (bool*) calloc(total_outputs, sizeof(bool));
  this->check_button_pins = (uint8_t*) malloc(sizeof(uint8_t)*total_check_buttons);
  this->radio_button_pins = (uint8_t*) malloc(sizeof(uint8_t)*total_radio_buttons);
  this->output_tracks = (uint8_t*) malloc(sizeof(uint8_t)*total_outputs);
  this->output_pins = (uint8_t*) malloc(sizeof(uint8_t)*total_outputs);
  this->bCur_pressed_check = (bool*) malloc(sizeof(bool)*total_check_buttons);
  this->bPrev_pressed_check = (bool*) calloc(total_check_buttons, sizeof(bool));
  this->bCur_pressed_radio = (bool*) malloc(sizeof(bool)*total_radio_buttons);
  this->bPrev_pressed_radio = (bool*) calloc(total_radio_buttons, sizeof(bool));
  
  // Initialise check buttons (toggled when pressed)
  uint8_t i;
  this->total_check_buttons = total_check_buttons;
  for (i = 0; i < total_check_buttons; i++) {
    (this->check_button_pins)[i] = check_button_pins[i]; 
    pinMode(check_button_pins[i], INPUT);
  }

  // Initialise radio buttons (activated when pressed, deactivates others)
  this->total_radio_buttons = total_radio_buttons;
  for (i = 0; i < total_radio_buttons; i++) {
    (this->radio_button_pins)[i] = radio_button_pins[i]; 
    pinMode(radio_button_pins[i], INPUT);
  }
  
  // Initialise outputs
  this->total_outputs = total_outputs;
  for (i = 0; i < total_outputs; i++) {
    (this->output_tracks)[i] = output_tracks[i];
    (this->output_pins)[i] = output_pins[i]; 
    pinMode(output_pins[i], OUTPUT);
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

}

void Toggle_buttons::update() 
{
  for (uint8_t i = 0; i < total_outputs; i++) {
    bToggled[i] = false;
  }

  update_check_buttons();
  update_radio_buttons();
  send_to_outputs();  
}

inline void Toggle_buttons::update_check_buttons()
{
  // Check buttons (each button toggles).
  for (uint8_t i = 0; i < total_check_buttons; i++) {
    bToggled[i] = false;

    // Button pulls input low.
    bCur_pressed_check[i] = !digitalRead(check_button_pins[i]);

    if (!bCur_pressed_check[i] && bPrev_pressed_check[i]) {
      bToggled[i] = true;
      // True if newly in active state (note "=" instead of "==")
      bActive[i] = !bActive[i];
    }

    bPrev_pressed_check[i] = bCur_pressed_check[i];
  }
}

inline void Toggle_buttons::update_radio_buttons()
{
  // Radio button (pressing this button activates this button,
  // and deactivates all other buttons.)
  uint8_t i;
  bool bRadio_toggled = false;
  uint8_t new_active_button;

  // Find toggled radio button, if there is one.
  for (i = 0; i < total_radio_buttons; i++) {
    // Button pulls input low.
    bCur_pressed_radio[i] = !digitalRead(radio_button_pins[i]);

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
  for (uint8_t i=0; i < total_outputs; i++) {
    if (bToggled[i]) {
      digitalWrite(output_pins[i], bActive[i]);
      
      // Write data.
      if (bOutput_track_in_message) {
        uint8_t true_message_length = (output_tracks[i] >= 10) ? 
          message_length + 2 : message_length + 1;

        Serial.write(true_message_length);
        Serial.write(message_part1);
        Serial.print(output_tracks[i]);
        Serial.write(message_part2);
        Serial.write('\0');
        bActive[i] ? Serial.write(1) : Serial.write(0);
        Serial.write('\0');
      } else {
        Serial.write(message_length);
        Serial.write(message_part1);
        Serial.write('\0');
        bActive[i] ? Serial.write(1) : Serial.write(0);
        Serial.write('\0');
      }
    }
  }
}

