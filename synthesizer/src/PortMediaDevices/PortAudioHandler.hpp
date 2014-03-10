#include "portaudio.h"
#include "AudioDevice.hpp"
#include <stdio.h>

namespace chip {
	
	class PortAudioHandler {
		public:
			// Setup and start a PortAudio Stream
			PaError connectAudioStream(int bufferSize, int sampleRate,
					PaDeviceIndex outDevID, PaDeviceIndex inDevID,
					int numOutChannels, int numInChannels, void *userData);

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
