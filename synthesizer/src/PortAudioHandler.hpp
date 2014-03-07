#include "portaudio.h"
#include <stdio.h>

namespace chip {
	
	class PortAudioHandler {
		public:
			// Setup and start a PortAudio Stream
			PaError ConnectAudioStream(int bufferSize, int sampleRate,
					PaDeviceIndex devID, int numChannels, void *userData);

			// Stop a PortAudio stream
			PaError DisconnectAudioStream();

			// PortAudio Callback
			static int paCallback(const void *inputBuffer, void *outputBuffer,
					unsigned long framesPerBuffer,
					const PaStreamCallbackTimeInfo* timeInfo,
					PaStreamCallbackFlags statusFlags, void *userData);

			void static printAudioDevices();
		private:

			// PortAudio Error Check
			PaError errorPortAudio(PaError err);

			// Declare stream
			PaStream *stream;
			PaError err;

	};

}
