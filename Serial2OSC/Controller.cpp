#include "Controller.h"

Controller::Controller() {
	t = std::unique_ptr<OSCConnection>(new OSCConnection());

	bRecording = false;
	bTrack_armed = false;
	cur_armed_track = 0;
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
			std::cout << "Faders yet to be implemented" << std::endl;
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

