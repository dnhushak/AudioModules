#include "Module.hpp"

chip::Module::Module()
{ 
	//constructor 
	
	//instantiates "bucket" of polyvoices
	polyvoices = new std::vector<chip::PolyVoice>();
	
	// Create the 127 polyvoices for the specific module and adds them to the bucket of polyvoices for that module
    for(int i = 0; i < 5; i++)
    {
        chip::PolyVoice* polyvoice = new chip::PolyVoice();
        polyvoices->push_back(*polyvoice);
    }
	
}

std::vector<float> chip::Module::advance(int numSamples)
{
	return mixer.advance(numSamples);
}
