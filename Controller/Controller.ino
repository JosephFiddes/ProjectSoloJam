#include <Arduino.h>
#include "toggle_buttons.h"
#include "sliders.h"

inline void update_values(bool digital_in[], const uint8_t total_digital_in,
  const uint8_t digital_in_pins[], const uint8_t total_digital_in_pins,
  int analog_in[], const uint8_t total_analog_in,
  const uint8_t analog_in_pins[], const uint8_t total_analog_in_pins,
  const uint8_t mux_toggle_pin); 

// Pin designations:
#define SENSOR_MUX_TOGGLE_PIN 2
#define DISPLAY_MUX_TOGGLE_PIN 3

#define TOTAL_DIGITAL_IN_PINS 4
const uint8_t digital_in_pins[TOTAL_DIGITAL_IN_PINS] = {9, 8, A4, A5};

#define TOTAL_ANALOG_IN_PINS 4
const uint8_t analog_in_pins[TOTAL_ANALOG_IN_PINS] = {A0, A1, A2, A3};

#define TOTAL_DIGITAL_IN_VALUES (TOTAL_DIGITAL_IN_PINS * 2)
#define TOTAL_ANALOG_IN_VALUES (TOTAL_ANALOG_IN_PINS * 2)

#define TOTAL_RECARM_BUTTONS 7

// Buttons for arming each track (note: all radio buttons)
Toggle_buttons track_recarm_buttons = Toggle_buttons(
  TOTAL_RECARM_BUTTONS, 1, false
);

#define TOTAL_RECORD_BUTTONS 1

// Button for toggling recording (note: check button)
Toggle_buttons record_buttons = Toggle_buttons(
  TOTAL_RECORD_BUTTONS, 0, false
);

/*
#define TOTAL_TRACK_VOL_SLIDERS 8
#define TOTAL_TRACKS_FOR_VOL_SLIDERS 8
const uint8_t track_numbers_for_vol_sliders[TOTAL_TRACKS_FOR_VOL_SLIDERS] 
  = {1, 2, 3, 4, 5, 6, 7, 8};

Sliders track_vol_sliders = Sliders(
  TOTAL_TRACK_VOL_SLIDERS,
  "n/track/",
  true, track_numbers_for_vol_sliders, TOTAL_TRACKS_FOR_VOL_SLIDERS,
  "/volume"
);
*/
// Effects are applied PreFader, including ReaStream. Therefore we can't
// adjust the master fader directly.
// Instead, all tracks are sent to a "master track", which then sends its output
// post-fader to the real master track.
// #define TOTAL_MASTER_VOL_SLIDERS 1
// 
// Sliders master_vol_sliders = Sliders(
//   TOTAL_MASTER_VOL_SLIDERS,
//   "n/master/volume"
// );

bool* digital_in_values;
int* analog_in_values; 

void setup() {
  Serial.begin(9600);

  for (uint8_t i = 0; i<TOTAL_ANALOG_IN_PINS; i++) pinMode(analog_in_pins[i], INPUT);
  for (uint8_t i = 0; i<TOTAL_DIGITAL_IN_PINS; i++) pinMode(digital_in_pins[i], INPUT);

  pinMode(SENSOR_MUX_TOGGLE_PIN, OUTPUT);
  pinMode(DISPLAY_MUX_TOGGLE_PIN, OUTPUT);

  digital_in_values = (bool*) malloc(sizeof(bool)*TOTAL_DIGITAL_IN_VALUES);
  analog_in_values = (int*) malloc(sizeof(int)*TOTAL_ANALOG_IN_VALUES);
}

void loop() {
  update_values(digital_in_values, TOTAL_DIGITAL_IN_VALUES,
    digital_in_pins, TOTAL_DIGITAL_IN_PINS,
    analog_in_values, TOTAL_ANALOG_IN_VALUES,
    analog_in_pins, TOTAL_ANALOG_IN_PINS,
    SENSOR_MUX_TOGGLE_PIN);

/*
  for (uint8_t i = 0; i<TOTAL_DIGITAL_IN_VALUES; i++) {
    Serial.print(digital_in_values[i]);
    Serial.print(", ");
  }
  Serial.println();
  

  for (uint8_t i = 0; i<TOTAL_ANALOG_IN_VALUES; i++) {
    Serial.print(analog_in_values[i]);
    Serial.print(", ");
  }
  Serial.println();

  Serial.flush();
*/


  bool bChanged_recarm = track_recarm_buttons.update(digital_in_values + 1);

  bool bChanged_record = record_buttons.update(digital_in_values);

  // track_vol_sliders.update(analog_in_values);
  // master_vol_sliders.update(analog_in_values);
}

inline void update_values(bool digital_in[], const uint8_t total_digital_in,
  const uint8_t digital_in_pins[], const uint8_t total_digital_in_pins,
  int analog_in[], const uint8_t total_analog_in,
  const uint8_t analog_in_pins[], const uint8_t total_analog_in_pins,
  const uint8_t mux_toggle_pin) 
{
  // First pass MUX 1
  digitalWrite(mux_toggle_pin, 1);
  delayMicroseconds(1);

  uint8_t i;
  for (i=0; i < total_digital_in_pins; i++) {
    digital_in[2*i] = digitalRead(digital_in_pins[i]);
  }

  for (i=0; i < total_analog_in_pins; i++) {
    analog_in[2*i] = analogRead(analog_in_pins[i]);
  }

  // Second pass MUX 0
  digitalWrite(mux_toggle_pin, 0);
  delayMicroseconds(1);

  for (i=0; i < total_digital_in_pins; i++) {
    digital_in[2*i + 1] = digitalRead(digital_in_pins[i]);
  }

  for (i=0; i < total_analog_in_pins; i++) {
    analog_in[2*i + 1] = analogRead(analog_in_pins[i]);
  }
}
