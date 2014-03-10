#include "PortAudioHandler.hpp"

namespace chip {

	// Setup and start a PortAudio Stream
	PaError PortAudioHandler::connectAudioStream(int bufferSize, int sampleRate,
			PaDeviceIndex outDevID, PaDeviceIndex inDevID, int numOutChannels,
			int numInChannels, void *userData) {

		// Declare output parameters
		PaStreamParameters outputParameters, inputParameters;

		// Initialize PA
		err = Pa_Initialize();
		if (err != paNoError)
			return errorPortAudio(err);

		// Set output Parameters
		outputParameters.device = outDevID;
		if (outputParameters.device == paNoDevice)
			return errorPortAudio(err);
		outputParameters.channelCount = numOutChannels;
		outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
		outputParameters.suggestedLatency = Pa_GetDeviceInfo(
				outputParameters.device)->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;

		// Set input Parameters
		inputParameters.device = inDevID;
		if (inputParameters.device == paNoDevice)
			return errorPortAudio(err);
		inputParameters.channelCount = numInChannels;
		inputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
		inputParameters.suggestedLatency = Pa_GetDeviceInfo(
				inputParameters.device)->defaultLowOutputLatency;
		inputParameters.hostApiSpecificStreamInfo = NULL;

		// Open the stream
		err = Pa_OpenStream(&astream, &inputParameters, &outputParameters,
				sampleRate, bufferSize, paNoFlag, /* we won't output out of range samples so don't bother clipping them */
				PortAudioHandler::paCallback, userData); // We want to pass a pointer to the AudioProcessor
		if (err != paNoError)
			return errorPortAudio(err);

		// Start the stream
		err = Pa_StartStream(astream);
		if (err != paNoError)
			return errorPortAudio(err);

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
		fprintf(stderr, "An error occured while using the portaudio stream\n");
		fprintf(stderr, "Error number: %d\n", err);
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
		printAudioDevices();
		return err;
	}

	// Print a list of valid devices
	void PortAudioHandler::printAudioDevices() {
		printf("***Valid Audio Devices: ***\n");
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
				printf("%d: %s\n", i, info->name);
			}

			PaDeviceIndex defaultin = Pa_GetDefaultInputDevice();
			PaDeviceIndex defaultout = Pa_GetDefaultOutputDevice();
			const PaDeviceInfo * inputinfo = Pa_GetDeviceInfo(
					(PaDeviceIndex) defaultin);
			const PaDeviceInfo * outputinfo = Pa_GetDeviceInfo(
					(PaDeviceIndex) defaultout);
			printf("\nDefault Input Device:   %d: %s\n", defaultin,
					inputinfo->name);
			printf("Default Output Device:  %d: %s\n\n", defaultout,
					outputinfo->name);
		}
	}

	// For any input stream we may want
	PaStream * PortAudioHandler::getStream() {
		return astream;
	}

	// PortAudio Callback
	int PortAudioHandler::paCallback(const void *inputBuffer,
			void *outputBuffer, unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags, void *userData) {
		// Cast void type outbut buffer to float
		float *out = (float*) outputBuffer;

		// Grab the supplied user data
		chip::AudioDevice * audio = (chip::AudioDevice*) userData;

		// Fill the output buffer
		out = audio->advance(framesPerBuffer);

		// Continue
		return paContinue;
	}

}
