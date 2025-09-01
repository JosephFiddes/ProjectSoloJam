#ifndef JDF_S2OSC_CONTROLLER
#define JDF_S2OSC_CONTROLLER

#include "OSCconnection.h"
#include <string>
#include <memory>

// This class contains the controller's internal state,
// as well as the process for sending controls to REAPER.
class Controller {
public:
	Controller();

	// Change controller's internal state (but don't send to REAPER yet).
	void add_command(char type, uint32_t source, uint32_t value);

	// Send commands to reaper.
	void send_commands();

private:
	std::unique_ptr<OSCConnection> t;

	bool bRecording;
	bool bTrack_armed;
	uint32_t cur_armed_track;
	bool bPlaying;
	bool bMetronome;

	inline void perform_button_action(uint32_t source);

	inline void toggle_recording();
	inline void set_record_arm(uint32_t track);
	inline void toggle_play();
	inline void toggle_metronome();

	inline void perform_fader_action(uint32_t source, float value);

	inline void set_track_volume(uint32_t track, float volume);
};


#endif