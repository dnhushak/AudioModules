#include "Module.hpp"

chip::Module::Module()
{ 
	//constructor
	next = 0;
	
	//instantiates the mixer
	mixer = new Mixer();
	
	//instantiates "bucket" of polyvoices
	polyvoices = new std::vector<chip::PolyVoice>();
	
	//instantiates the list of active polyvoices
	activeNotes = new std::vector<int>(0, 0);
	
	// Create the voice for this module
	setVoice(100, 1000, 0.5, 500);
	
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
	
	int note;
	
	//for(int i = 0; i < NUM_POLYVOICES; i++)
	for(unsigned int i = 0; i < (*activeNotes).size(); i++)
	{
	    note = (*activeNotes)[i];
	    
	    if((*polyvoices)[note].getState() == OFF)
        {
            releasePolyVoice(i);
            continue;
        }
	    
	    //for each active IAudio in audioList, advance
	    *temp = (*polyvoices)[note].advance(numSamples);
		for(int j = 0; j < numSamples; j++)
		{
		    //sum each advanced IAudio to the master mixed vector
			(*mixedFinal)[j] = (*mixedFinal)[j] + (*temp)[j];
		}
	}
	
	temp->clear();
	delete temp;
	
	return *mixedFinal; //the final, "synthesized" list
}

void chip::Module::setVoice(int attack, int decay, float sustain, int release)
{
    voice = new Voice(attack, decay, sustain, release);
}

void chip::Module::activatePolyVoice(int note)
{
    /*(*polyvoices)[next].note = note;
    (*polyvoices)[next].phase = 0.0;
    (*polyvoices)[next].frequency = MtoF(note);
    (*polyvoices)[next].state = ATTACK;
    (*polyvoices)[next].setVoice(voice->getAttack(), 
                                 voice->getDecay(), 
                                 voice->getSustain(),
                                 voice->getRelease());
    
    next++;*/
    
    (*polyvoices)[note].note = note;
    (*polyvoices)[note].phase = 0.0;
    (*polyvoices)[note].frequency = MtoF(note);
    (*polyvoices)[note].state = ATTACK;
    (*polyvoices)[note].setVoice(voice->getAttack(), 
                                 voice->getDecay(), 
                                 voice->getSustain(),
                                 voice->getRelease());
                                 
    (*activeNotes).push_back(note);
}

void chip::Module::releasePolyVoice(int index)
{
    // Find a matching note and swap it with the last active polyvoice (next - 1).
    // By setting the to-be-deactivated polyvoice to the last active polyvoice and 
    // deactivating the last active polyvoice, we are swapping the two.
    /*for(int i = 0; i < NUM_POLYVOICES; i++)
    {
        if((*polyvoices)[i].note == note)
        {
            (*polyvoices)[i].note = (*polyvoices)[next-1].note;
            (*polyvoices)[i].phase = (*polyvoices)[next-1].phase;
            (*polyvoices)[i].frequency = (*polyvoices)[next-1].frequency;
            (*polyvoices)[i].state = (*polyvoices)[next-1].getState();
            (*polyvoices)[i].setEnvmult((*polyvoices)[next-1].getEnvmult());
            (*polyvoices)[i].setEnvloc((*polyvoices)[next-1].getEnvloc());
            
            (*polyvoices)[next-1].releasePolyVoice();
            
            next--;
            
            break;
        }
    }*/
    int temp;
    
    if((*activeNotes).size() > 0) { 
        temp = (*activeNotes)[index];
        (*activeNotes)[index] = (*activeNotes)[(*activeNotes).size() - 1];
        (*activeNotes)[(*activeNotes).size() - 1] = temp;
        (*activeNotes).pop_back();
    }
}


void chip::Module::cleanup()
{
    for(int i = 0; i < next; i++)
    {
        // Find any note that is flagged for "clean up" and swap it with the
        // last active polyvoice (next - 1).
        if((*polyvoices)[i].state == CLEANUP)
        {
            (*polyvoices)[i].note = (*polyvoices)[next-1].note;
            (*polyvoices)[i].phase = (*polyvoices)[next-1].phase;
            (*polyvoices)[i].frequency = (*polyvoices)[next-1].frequency;
            (*polyvoices)[i].state = (*polyvoices)[next-1].getState();
            
            (*polyvoices)[next-1].state = OFF;
            
            next--;
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
