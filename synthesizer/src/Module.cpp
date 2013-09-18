#include "Module.hpp"

chip::Module::Module()
{ 
	//constructor
	
	//instantiates the mixer
	mixer = new Mixer();
	
	//instantiates "bucket" of polyvoices
	polyvoices = new std::vector<chip::PolyVoice>();
	
	// Create the 127 polyvoices for the specific module and adds them to the bucket of polyvoices for that module
    for(int i = 0; i < NUM_POLYVOICES; i++)
    {
        chip::PolyVoice* polyvoice = new chip::PolyVoice();
        mixer->addObjects((IAudio*)polyvoice);
        polyvoices->push_back(*polyvoice);
    }
    
    std::cout << "Module created num polyvoices = " << NUM_POLYVOICES << "\n";
}

std::vector<float> chip::Module::advance(int numSamples)
{
	return mixer->advance(numSamples);
}

void chip::Module::activatePolyVoice(int index, int note, float phase, int frequency)
{
    (*polyvoices)[index].note = note;
    (*polyvoices)[index].phase = phase;
    (*polyvoices)[index].frequency = frequency;
    (*polyvoices)[index].isActive = 1;
}

void chip::Module::deactivatePolyVoice(int index)
{
    (*polyvoices)[index].isActive = 0;
}

void chip::Module::printPolyVoices()
{
    for(int i = 0; i < 127; i++)
    {
        std::cout << "polyvoice" << i << ": " << (*polyvoices)[i].isActive << "\n";
    }
}
