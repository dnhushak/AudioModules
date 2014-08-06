#pragma once
#include <sstream>
#include <string>

namespace modules {
	struct ChipVoice : Voice {
			/*** Module/Instrument level parameters go here: ***/
			bool arp_en;
			bool gliss_en;
			int arpTime;
			int glissTime;
			float volume;

			/*** Polyphonic/Note level parameters go here: ***/
			// Oscillator settings
			Wavetable * osc_table;
			int osc_attack;
			int osc_decay;
			float osc_sustain;
			int osc_release;

			// Vibrato Settings
			Wavetable * vib_table;
			int vib_time;
			bool vib_en;
	};
}
