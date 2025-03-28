#ifndef JDF_OSCCONT_SLIDERS
#define JDF_OSCCONT_SLIDERS

#include <Arduino.h>

typedef union {
  float fvalue;
  byte bits[4];
} Binary_float;

class Sliders {
public:
  Sliders(uint8_t total_sliders,
    const char* message_part1, 
    bool bOutput_track_in_message = false, 
    uint8_t track_numbers[] = {}, 
    uint8_t total_tracks = 1, 
    const char* message_part2 = "");
  void update(int sliders[]);

  int* values;
  bool* bToggled;
  uint8_t* track_numbers;

  uint8_t total_sliders;
  uint8_t total_tracks;

private:
  inline void update_sliders(int new_values[]);
  inline void send_to_outputs();

  /*
  bool* bCur_pressed_check;
  bool* bPrev_pressed_check;
  bool* bCur_pressed_radio;
  bool* bPrev_pressed_radio;
  */

  bool bOutput_track_in_message;
  uint8_t message_length;
  char* message_part1;
  char* message_part2;
};

#endif