#include "AudioProcessor.hpp"
#include "MIDIParser.hpp"


static chip::MIDIParser* midiParser;

// Buffer for sound data to be sent off through the audio callback
static std::vector<float> buffer;

/* This routine will be called by the PortAudio engine when audio is needed.
 ** It may called at interrupt level on some machines so don't do anything
 ** that could mess up the system like calling malloc() or free().
 */
static int paCallback( const void *inputBuffer, 
                       void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData );


/*
 * This function gets called whenever an error occurred while setting up
 * PortAudio.
 */
int errorPortAudio(int err) 
{
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}

/*
 * This routine is called by portaudio when playback is done.
 */
static void StreamFinished( void* userData )
{
      
}

/*
 * Sets up the PortAudio audio stream and then waits until the user presses
 * enter to terminate. During this time, audio callbacks are occurring.
 */
int main(int argc, char *argv[])
{
	//Grab midi device ID from arguments
	int devID;
	if (argc!=2){
		printf("Enter MIDI Device ID: ./Synthesizer #\n");
		return -1;
	}
	else{
		devID = atoi(argv[1]);
	}
	
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    
    chip::AudioProcessor* audioProcessor = new chip::AudioProcessor();
    midiParser = new chip::MIDIParser();
    
    // TODO Instead of passing each of the modules, just pass the vector
    // Give the MIDIParser pointers to the modules
    for(int i = 0; i < 5; i++)
    {
        midiParser->addObject((*(audioProcessor->modules))[i]);
    }
    
    err = Pa_Initialize();
    if( err != paNoError ) errorPortAudio(err);
    
    //outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.device = 0;
    std::cout << Pa_GetDeviceInfo(outputParameters.device)->name;
    if (outputParameters.device == paNoDevice) errorPortAudio(err);
    
    outputParameters.channelCount = 1;       /* mono output? */ // TODO Shouldn't this be 5?
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
        
    err = Pa_OpenStream(
          &stream,
          NULL, /* no input */
          &outputParameters,
          SAMPLE_RATE,
          paFramesPerBufferUnspecified,
          paClipOff,      /* we won't output out of range samples so don't bother clipping them */
          paCallback,
          audioProcessor); // We want to pass a pointer to the AudioProcessor
          
    if( err != paNoError ) errorPortAudio(err);
    
    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) errorPortAudio(err);
    
    // Connect to the MIDI stream and start reading
    midiParser->connectToMIDIStream(devID);
    
    err = Pa_StartStream( stream );
    if( err != paNoError ) errorPortAudio(err);
    
	/*
print cpu usage every some seconds
*/
    while(1)
    {
            std::cout << "\r" << int(100.0f*Pa_GetStreamCpuLoad(stream)) << "% CPU         " << std::flush;
       usleep(20000);
    }

    // Block the front end until someone hits enter
    // We are getting audio callbacks while this is happening
    std::cin.ignore(255, '\n');
    
    err = Pa_StopStream( stream );
    if( err != paNoError ) errorPortAudio(err);

    midiParser->disconnectMIDIStream();

    err = Pa_CloseStream( stream );
    if( err != paNoError ) errorPortAudio(err);

    Pa_Terminate();
}

static int paCallback( const void *inputBuffer, 
                       void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData )
{
    midiParser->readMIDI();
    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    chip::AudioProcessor* audio = (chip::AudioProcessor*)userData;
    float *out = (float*)outputBuffer;
    
    // TODO make outside of callback
    buffer = audio->advance(framesPerBuffer);
    if(buffer[0] == 0)
    {
        (void) buffer;
    }
    
    for(int i = 0; i < framesPerBuffer; i++)
    {
        //std::cout << buffer[i] / 65536 << "\n";
        *out++ = buffer[i] / 65536;
        //std::cout << buffer[i];
    }
      
    //AudioProcessor::masterMixer.advance(FRAMES_PER_BUFFER);
        
    buffer.clear();
    return paContinue;
}


