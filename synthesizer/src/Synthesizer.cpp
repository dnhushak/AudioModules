#include "AudioProcessor.hpp"
#include "MIDIParser.hpp"



/* This routine will be called by the PortAudio engine when audio is needed.
 ** It may called at interrupt level on some machines so don't do anything
 ** that could mess up the system like calling malloc() or free().
 */
static int paCallback( const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData );


/*
 * This function gets called whenever an error occurred while setting up
 * PortAudio.
 */
int error(int err) 
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
int main(void)
{
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    //void data;
    
    chip::AudioProcessor* audioProcessor = new chip::AudioProcessor();
    
    std::cout << audioProcessor << "\n";
    std::cout << "&: " << &audioProcessor << "\n";
    
    chip::MIDIParser* midiParser = new chip::MIDIParser();
    
    // Give the MIDIParser pointers to the modules
    for(int i = 0; i < 5; i++)
    {
        midiParser->addObject((*(audioProcessor->modules))[i]);
    }
    
    err = Pa_Initialize();
    if( err != paNoError ) error(err);
    
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) error(err);
    
    outputParameters.channelCount = 1;       /* mono output? */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
        
    err = Pa_OpenStream(
          &stream,
          NULL, /* no input */
          &outputParameters,
          SAMPLE_RATE,
          FRAMES_PER_BUFFER,
          paClipOff,      /* we won't output out of range samples so don't bother clipping them */
          paCallback,
          audioProcessor); // We want to pass a pointer to the AudioProcessor
          
    if( err != paNoError ) error(err);
    
    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) error(err);
    
    err = Pa_StartStream( stream );
    if( err != paNoError ) error(err);
    
    midiParser->readMIDI();
    
    // Block the front end until someone hits enter
    // We are getting audio callbacks while this is happening
    std::cin.ignore(255, '\n');
    
    err = Pa_StopStream( stream );
    if( err != paNoError ) error(err);

    err = Pa_CloseStream( stream );
    if( err != paNoError ) error(err);

    Pa_Terminate();
}

static int paCallback( const void *inputBuffer, void *outputBuffer,
                                   unsigned long framesPerBuffer,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData )
{
    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    chip::AudioProcessor* audio = (chip::AudioProcessor*)userData;
    float *out = (float*)outputBuffer;
    
    std::vector<float> buffer = audio->advance(FRAMES_PER_BUFFER);
    if(buffer[0] == 0)
    {
        (void) buffer;
    }
    
    for(int i = 0; i < FRAMES_PER_BUFFER; i++)
    {
        //std::cout << buffer[i] / 65536 << "\n";
        *out++ = buffer[i] / 65536;
        //std::cout << buffer[i];
    }
      
    //AudioProcessor::masterMixer.advance(FRAMES_PER_BUFFER);
        
    return paContinue;
}


