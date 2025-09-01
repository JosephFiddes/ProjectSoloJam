#include "Controller.h"

//#define DEBUG 

Controller::Controller() {
	t = std::unique_ptr<OSCConnection>(new OSCConnection());

	bRecording = false;
	bTrack_armed = false;
	cur_armed_track = 0;
	bPlaying = false;
	bMetronome = true;
}

void Controller::add_command(char type, uint32_t source, uint32_t value) {
	// Initialise the buffer, if not already.
	if (!t->buffer_initialised()) {
		t->init_buffer();
	}

	switch (type) {
		case 'B': // Button
			perform_button_action(source);
			break;
		case 'F': // Fader
			// In this case, value is actually a float.
			perform_fader_action(source, *reinterpret_cast<float*>(&value));
			break;
		default:
			std::cerr << "Unexpected command type " << type << std::endl;
	}
}

void Controller::send_commands() {
    t->finalize_buffer();
    t->send();
}

inline void Controller::perform_button_action(uint32_t source) {
	#ifdef DEBUG
		std::cout << "B" << source << std::endl;
	#endif

	// Take differing action based on the particular source of the button press.
	// CASE 0: Toggle Recording.
	if (source == 0) {
		toggle_recording();
		return;
	}

	// CASE 1-7: Set record arm.
	if (source <= 7) {
		// Track number is button index + 1.
		set_record_arm(source + 1);
		return;
	}

	// Miscellaneous Functions (start on 0x100).
	switch (source) {
		case 0x101:
			toggle_play();
			return;
		case 0x102:
			toggle_metronome();
			return;
	}	
}

inline void Controller::toggle_recording() {
	bRecording = !bRecording;

	t->add_to_buffer("t/record", 1, 'i');
}

inline void Controller::set_record_arm(uint32_t track) {
	// Don't do anything if track is already armed.
	if (bTrack_armed && track == cur_armed_track) return;

	bTrack_armed = true;

	// Set the armed track (unset the previous track).
	uint32_t prev_armed_track = cur_armed_track;
	cur_armed_track = track;

	auto message = [](uint32_t track) {
		return "b/track/" + std::to_string(track) + "/recarm";
	};

	t->add_to_buffer(message(prev_armed_track).c_str(), 0, 'i');
	t->add_to_buffer(message(cur_armed_track).c_str(), 1, 'i');

	// Turn off recording in order to finalise prev track record.
	if (bRecording) toggle_recording();
}

inline void Controller::toggle_play() {
	bPlaying = !bPlaying;

	t->add_to_buffer("t/play", 1, 'i');
}

inline void Controller::toggle_metronome() {
	bMetronome = !bMetronome;

	t->add_to_buffer("t/click", 1, 'i');
}

inline void Controller::perform_fader_action(uint32_t source, float value) {
	#ifdef DEBUG
		std::cout << "F" << source << " " << value << std::endl;
	#endif

	// Track number is fader index + 1. Value == volume.
	set_track_volume(source + 1, value);
}

inline void Controller::set_track_volume(uint32_t track, float volume) {
	auto message = [](uint32_t track) {
		return "n/track/" + std::to_string(track) + "/volume";
	};

	// add_to_buffer takes a uint32_t for its second argument.
	// This will be converted back to a float inside the function.
	t->add_to_buffer(message(track).c_str(), 
		*reinterpret_cast<uint32_t*>(&volume), 'f');
}