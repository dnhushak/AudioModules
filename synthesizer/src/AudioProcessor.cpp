#include "AudioProcessor.hpp"

namespace chip 
{
    AudioProcessor::AudioProcessor()
    {
        int i;
        
        // Create the 5 modules for the synthesizer and add them to the mixer
        for(i = 0; i < 5; i++)
        {
            modules.push_back(new Module());
        }
        
        masterMixer = new Mixer(modules);
    }
    
    AudioProcessor::~AudioProcessor()
    {
       
    }
    
    static int paCallback( const void *inputBuffer, void *outputBuffer,
                                   unsigned long framesPerBuffer,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData )
    {
        float *out = (float*)outputBuffer;
        
        (void) inputBuffer;
        (void) timeInfo;
        (void) statusFlags;
        (void) userData;
        
        //AudioProcessor::masterMixer.advance(FRAMES_PER_BUFFER);
    }
}
