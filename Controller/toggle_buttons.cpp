#include "toggle_buttons.h"

Toggle_buttons::Toggle_buttons(uint8_t pins[]) {
  for (uint8_t i = 0; i < TOTAL_BUTTONS; i++) {
    (this->pins)[i] = pins[i]; 
    pinMode(pins[i], INPUT);
  }
}

void Toggle_buttons::update() {
  for (uint8_t i = 0; i < TOTAL_BUTTONS; i++) {
    bToggled[i] = false;

    // Button pulls input low.
    bCur_pressed[i] = !digitalRead(pins[i]);

    if (!bCur_pressed[i] && bPrev_pressed[i]) {
      bToggled[i] = true;
      // True if newly in active state (note "=" instead of "==")
      bActive[i] = !bActive[i];
    }

    bPrev_pressed[i] = bCur_pressed[i];
  }
}
