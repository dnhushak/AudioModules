#include "AudioProcessor.hpp"

namespace chip 
{
    AudioProcessor::AudioProcessor()
    {
        int i;
        masterMixer = new Mixer();
        
        // Create the 5 modules for the synthesizer and add them to the mixer
        for(i = 0; i < 5; i++)
        {
            IAudio* module = new Module();
            masterMixer->addObjects(module);
        }
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
