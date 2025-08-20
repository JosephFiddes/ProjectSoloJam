#ifndef JDF_OSCCONT_SLIDERS
#define JDF_OSCCONT_SLIDERS

#include <Arduino.h>

typedef union {
  float fvalue;
  byte bits[4];
} Binary_float;

class Sliders {
public:
  Sliders(uint8_t total_sliders, uint32_t first_index);
  void update(int sliders[]);

  int* values;
  bool* bToggled;

  uint8_t total_sliders;

private:
  inline void update_sliders(int new_values[]);
  inline void send_to_outputs();
  inline void send_message(const uint32_t index, const Binary_float value);

  uint32_t first_index;
};

#endif