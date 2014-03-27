#include "PortAudioHandler.hpp"

namespace chip {

	// Setup and start a PortAudio Stream
	PaError PortAudioHandler::connectAudioStream(int bufferSize, int sampleRate,
			PaDeviceIndex outDevID, PaDeviceIndex inDevID, int numOutChannels,
			int numInChannels, void *userData) {

		// Declare output parameters
		PaStreamParameters * outputParameters, *inputParameters;
		outputParameters = new PaStreamParameters;
		inputParameters = new PaStreamParameters;
		// Initialize PA
		err = Pa_Initialize();
		if (err != paNoError)
			return errorPortAudio(err);

		// Check if using output stream
		if (outDevID == -1) {
			printf("No output device selected...\n");
			outputParameters = NULL;
		} else {
			printf("Connecting to output device %d...\n", outDevID);
			// Set output Parameters
			outputParameters->device = outDevID;
			if (outputParameters->device == paNoDevice)
				return errorPortAudio(err);
			outputParameters->channelCount = numOutChannels;
			printf("Using %d channels...\n", numOutChannels);
			outputParameters->sampleFormat = paFloat32; /* 32 bit floating point output */
			outputParameters->suggestedLatency = Pa_GetDeviceInfo(
					outputParameters->device)->defaultLowOutputLatency;
			outputParameters->hostApiSpecificStreamInfo = NULL;
		}

		// Check if using input stream
		if (inDevID == -1) {
			printf("No input device selected...\n");
			inputParameters = NULL;
		} else {
			printf("Connecting to input device %d...\n", inDevID);
			// Set input Parameters
			inputParameters->device = inDevID;
			if (inputParameters->device == paNoDevice)
				return errorPortAudio(err);
			inputParameters->channelCount = numInChannels;
			printf("Using %d channels...\n", numInChannels);
			inputParameters->sampleFormat = paFloat32; /* 32 bit floating point output */
			inputParameters->suggestedLatency = Pa_GetDeviceInfo(
					inputParameters->device)->defaultLowOutputLatency;
			inputParameters->hostApiSpecificStreamInfo = NULL;
		}

		// Open the stream
		err = Pa_OpenStream(&astream, inputParameters, outputParameters,
				sampleRate, bufferSize, paNoFlag, /* we won't output out of range samples so don't bother clipping them */
				paCallback, userData); // We want to pass a pointer to the AudioProcessor
		if (err != paNoError)
			return errorPortAudio(err);
		printf("Stream opened...\n");
		// Start the stream
		err = Pa_StartStream(astream);
		if (err != paNoError)
			return errorPortAudio(err);
		printf("Stream started...\n");
		return err;
	}

	// Stop a PortAudio stream
	PaError PortAudioHandler::disconnectAudioStream() {
		PaError err;
		err = Pa_StopStream(astream);
		if (err != paNoError)
			errorPortAudio(err);

		err = Pa_CloseStream(astream);
		if (err != paNoError)
			errorPortAudio(err);

		return err;
		Pa_Terminate();

	}

	// PortAudio Error Check
	PaError PortAudioHandler::errorPortAudio(PaError err) {
		Pa_Terminate();
		std::string red = "\033[1;31m";
		std::string defcol = "\033[0m";
		std::cout << red;
		std::cout << "An error occured while using the portaudio stream\n";
		std::cout << "Error number: " << err << "\n";
		std::cout << "Error message: " << Pa_GetErrorText(err) << "\n";
		std::cout << defcol;
		printAudioDevices();
		return err;
	}

	// Print a list of valid devices
	void PortAudioHandler::printAudioDevices() {
		//Make Magenta
		std::string magenta = "\033[1;35m";
		// Make default color
		std::string defcol = "\033[0m";
		std::cout << magenta << "***Valid Audio Devices: ***\n" << defcol;
		Pa_Initialize();
		int ndev;
		ndev = Pa_GetDeviceCount();
		if (ndev == 0) {
			printf("No Audio devices connected!\n");
		} else {
			for (int i = 0; i < ndev; i++) {
				const PaDeviceInfo * info = Pa_GetDeviceInfo((PaDeviceIndex) i);
				if (info->maxOutputChannels > 0) {

					if (info->maxInputChannels > 0) {
						printf("Input/Output Device:    ");
					} else {
						printf("Output Device:          ");
					}
				} else {
					if (info->maxInputChannels > 0) {
						printf("Input Device:           ");
					} else {
						printf("Device:                 ");
					}

				}
				printf("| In: %02i | Out: %02i | %02i: %s \n",
						info->maxInputChannels, info->maxOutputChannels, i,
						info->name);
			}

			std::cout << magenta << "***Default Audio Devices: ***\n" << defcol;

			PaDeviceIndex defaultin = Pa_GetDefaultInputDevice();
			PaDeviceIndex defaultout = Pa_GetDefaultOutputDevice();
			const PaDeviceInfo * inputinfo = Pa_GetDeviceInfo(
					(PaDeviceIndex) defaultin);
			const PaDeviceInfo * outputinfo = Pa_GetDeviceInfo(
					(PaDeviceIndex) defaultout);
			printf(
					"Default Input Device:   | In: %02i | Out: %02i | %02i: %s \n",
					inputinfo->maxInputChannels, inputinfo->maxOutputChannels,
					defaultin, inputinfo->name);
			printf(
					"Default Output Device:  | In: %02i | Out: %02i | %02i: %s \n\n",
					outputinfo->maxInputChannels, outputinfo->maxOutputChannels,
					defaultout, outputinfo->name);
		}
	}

// For any input stream we may want
	PaStream * PortAudioHandler::getStream() {
		return astream;
	}

// PortAudio Callback
	int PortAudioHandler::paCallback(const void * inputBuffer,
			void * outputBuffer, unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags, void *userData) {

		//TODO: get multichannel to work right
		// Cast void type output buffer to float
		float * out = (float*) outputBuffer;

		// Grab the supplied user data
		chip::AudioDevice * audio = (chip::AudioDevice*) userData;

		// Fill the output buffer
		float * buffer = audio->advance(framesPerBuffer);
		for (int i = 0; i < framesPerBuffer; i++) {
			*out++ = buffer[i];
		}
		// Continue
		return paContinue;
	}

}
