#include "portaudio.h"
#include "AudioDevice.h"
#include <stdio.h>

//Define Terminal Text Colors (for printing)
#define magenta "\033[1;35m"
#define defcol "\033[0m"
#define red "\033[1;31m"

namespace audio {
	
	class PortAudioHandler : AudioDevice {
		public:
			virtual PortAudioHandler * clone(int);

			sample_t * advance();

			// Setup and start a PortAudio Stream
			PaError connectAudioStream(PaDeviceIndex outDevID,
					PaDeviceIndex inDevID, int numOutChannels,
					int numInChannels, void *userData);

			// Stop a PortAudio stream
			PaError disconnectAudioStream();

			// PortAudio Callback
			static int paCallback(const void *inputBuffer, void *outputBuffer,
					unsigned long framesPerBuffer,
					const PaStreamCallbackTimeInfo* timeInfo,
					PaStreamCallbackFlags statusFlags, void *userData);

			void printAudioDevices();

			PaStream * getStream();
		private:
			// PortAudio Error Check
			PaError errorPortAudio(PaError err);

			// Declare stream
			PaStream * astream;
			PaError err;

	};

}
