#include "sliders.h"

//#define DEBUG

#define SLIDER_THRESHOLD 20
#define NORM_FACTOR (1.0 / 1024)

Sliders::Sliders(uint8_t total_sliders, uint32_t first_index)
{
  // Allocate memory.
  this->values = (int*) calloc(total_sliders, sizeof(int));
  this->bToggled = (bool*) calloc(total_sliders, sizeof(bool));

  this->total_sliders = total_sliders;
  this->first_index = first_index;
}

void Sliders::update(int new_values[])
{
  update_sliders(new_values);
  send_to_outputs();  
}

inline void Sliders::update_sliders(int new_values[])
{
  int diff;

  // Check if slider has sufficiently changed.
  for (uint8_t i = 0; i < total_sliders; i++) {
    bToggled[i] = false;

    diff = abs(values[i] - new_values[i]);
    if (diff >= SLIDER_THRESHOLD) {
      bToggled[i] = true;
      values[i] = new_values[i];
    }
  }
}

inline void Sliders::send_to_outputs() 
{
  Binary_float normalized_value;

  for (uint8_t i=0; i < total_sliders; i++) {
    if (bToggled[i]) {
      // OSC expects 32-bit big-endian IEEE 754 floating point number 
      normalized_value.fvalue = values[i] * NORM_FACTOR;

      send_message(first_index + i, normalized_value);
    }
  }
}

inline void Sliders::send_message(const uint32_t index, const Binary_float value) {
  Serial.write('F');
  #ifdef DEBUG  
    Serial.print(index);
    Serial.print(value.fvalue);
    Serial.println();
  #else
    Serial.write((byte*) &index, 4);
    Serial.write((byte*) &(value.bits), 4);
    Serial.flush();
  #endif
}