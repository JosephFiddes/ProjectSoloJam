#include "toggle_buttons.h"

Toggle_buttons::Toggle_buttons(uint8_t check_button_pins[], uint8_t radio_button_pins[]) {
  uint8_t i;
  for (i = 0; i < TOTAL_CHECK_BUTTONS; i++) {
    (this->check_button_pins)[i] = check_button_pins[i]; 
    pinMode(check_button_pins[i], INPUT);
  }

  for (i = 0; i < TOTAL_RADIO_BUTTONS; i++) {
    (this->radio_button_pins)[i] = radio_button_pins[i]; 
    pinMode(radio_button_pins[i], INPUT);
  }
}

void Toggle_buttons::update() {
  uint8_t i;

  for (i = 0; i < TOTAL_BUTTONS; i++) {
    bToggled[i] = false;
  }

  // Check buttons (each button toggles).
  for (i = 0; i < TOTAL_CHECK_BUTTONS; i++) {
    bToggled[i] = false;

    // Button pulls input low.
    bCur_pressed[i] = !digitalRead(check_button_pins[i]);

    if (!bCur_pressed[i] && bPrev_pressed[i]) {
      bToggled[i] = true;
      // True if newly in active state (note "=" instead of "==")
      bActive[i] = !bActive[i];
    }

    bPrev_pressed[i] = bCur_pressed[i];
  }

  // Radio button (pressing this button activates this button,
  // and deactivates all other buttons.)
  bool bRadio_toggled = false;
  uint8_t new_active_button;
  for (i = 0; i < TOTAL_RADIO_BUTTONS; i++) {
    // Button pulls input low.
    bCur_pressed[i] = !digitalRead(radio_button_pins[i]);

    // Button up event.
    if (!bCur_pressed[i] && bPrev_pressed[i]) {
      bRadio_toggled = true;
      new_active_button = i;
    }

    bPrev_pressed[i] = bCur_pressed[i];
  }

  if (bRadio_toggled) {
    for (i = 0; i < TOTAL_RADIO_BUTTONS; i++) {
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
