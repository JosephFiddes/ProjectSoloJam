#include "sliders.h"

//#define DEBUG

#define SLIDER_THRESHOLD 20
#define NORM_FACTOR (1.0 / 1024)

Sliders::Sliders(uint8_t total_sliders,
  const char* message_part1, 
  bool bOutput_track_in_message, 
  uint8_t track_numbers[], 
  uint8_t total_tracks, 
  const char* message_part2)
{
  // Allocate memory.
  this->values = (int*) calloc(total_tracks, sizeof(int));
  this->bToggled = (bool*) calloc(total_tracks, sizeof(bool));
  this->track_numbers = (uint8_t*) malloc(sizeof(uint8_t)*total_tracks);
  
  // Initialise check buttons (toggled when pressed)
  this->total_sliders = total_sliders;

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
}

void Sliders::update(int new_values[])
{
  for (uint8_t i = 0; i < total_tracks; i++) {
    bToggled[i] = false;
  }

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

  for (uint8_t i=0; i < total_tracks; i++) {
    if (bToggled[i]) {
      // OSC expects 32-bit big-endian IEEE 754 floating point number 
      normalized_value.fvalue = values[i] * NORM_FACTOR;

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
    
      Serial.write(normalized_value.bits, 4);
      Serial.write('f');
      Serial.write('\0');

    #ifdef DEBUG  
      Serial.println();
    #else
      Serial.flush();
    #endif
    }
  }
}

