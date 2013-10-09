#include "Module.hpp"

chip::Module::Module()
{ 
	//constructor
	next = 0;
	
	//instantiates the mixer
	mixer = new Mixer();
	
	//instantiates "bucket" of polyvoices
	polyvoices = new std::vector<chip::PolyVoice>();
	
	// Create the voice for this module
	setVoice(100, 1000, 0.5, 1000);
	
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
    //the 0th elements are all added together, the 1st elements, 2nd, all the way to the 
	//nth elements and the result is returned (aka - move along the sound wave)
	std::vector<float>* mixedFinal = new std::vector<float>(numSamples, 0.0);
	std::vector<float>* temp = new std::vector<float>(numSamples, 0.0);
	
	for(int i = 0; i < next; i++)
	{ 
	    //for each IAudio in audioList, advance
	    *temp = (*polyvoices)[i].advance(numSamples);
		for(int j = 0; j < numSamples; j++)
		{
		    //sum each advanced IAudio to the master mixed vector
			(*mixedFinal)[j] = (*mixedFinal)[j] + (*temp)[j];
		}
	}
	return *mixedFinal; //the final, "synthesized" list
}

void chip::Module::setVoice(int attack, int decay, float sustain, int release)
{
    voice = new Voice(attack, decay, sustain, release);
}

void chip::Module::activatePolyVoice(int note)
{
    (*polyvoices)[next].note = note;
    (*polyvoices)[next].phase = 0.0;
    (*polyvoices)[next].frequency = MtoF(note);
    (*polyvoices)[next].state = ATTACK;
    (*polyvoices)[next].setVoice(voice->getAttack(), 
                                    voice->getDecay(), 
                                    voice->getSustain(),
                                    voice->getRelease());
    
    next++;
}

void chip::Module::deactivatePolyVoice(int note)
{
    // Find a matching note and "swap" it with the last active polyvoice (next - 1).
    // By setting the to-be-deactivated polyvoice to the last active polyvoice and 
    // deactivating the last active polyvoice, we are "swapping" the two.
    for(int i = 0; i < NUM_POLYVOICES; i++)
    {
        if((*polyvoices)[i].note == note)
        {
            (*polyvoices)[i].note = (*polyvoices)[next-1].note;
            (*polyvoices)[i].phase = (*polyvoices)[next-1].phase;
            (*polyvoices)[i].frequency = (*polyvoices)[next-1].frequency;
            (*polyvoices)[i].state = ATTACK;
            
            (*polyvoices)[next-1].state = RELEASE;
            
            next--;
            
            break;
        }
    }
}

//Midi Note to Frequency
float chip::Module::MtoF(int note){
	return pow(2,(((float)note - 69)/12.0))*440.0;
}

void chip::Module::printPolyVoices()
{
    for(int i = 0; i < NUM_POLYVOICES; i++)
    {
        std::cout << "polyvoice" << i << ": " << (*polyvoices)[i].state << "\n";
    }
}
