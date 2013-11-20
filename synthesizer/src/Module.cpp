#include "Module.hpp"

chip::Module::Module()
{
    Module(new Voice(1, 1, 0.5, 1, SQUARE));
}

chip::Module::Module(Voice* voice)
{ 
	//constructor
	this->next = 0;
	
	//instantiates the mixer
	this->mixer = new Mixer();
	
	//instantiates "bucket" of polyvoices
	this->polyvoices = new std::vector<chip::PolyVoice>();
	
	// Create the voice for this module
	this->voice = voice;
	
	// Set arpeggiation and glissando
	this->arpeggio = 50;
	
	// Create the 127 polyvoices for the specific module and adds them to the bucket of polyvoices for that module
    for(int i = 0; i < NUM_POLYVOICES; i++)
    {
        //void (*fptr)(int note) = removePolyVoice;
        //chip::PolyVoice* polyvoice = new chip::PolyVoice(removePolyVoice);
        chip::PolyVoice* polyvoice = new chip::PolyVoice();
        mixer->addObjects((IAudio*)polyvoice);
        polyvoices->push_back(*polyvoice);
    }
}

void chip::Module::setVoice(int attack, int decay, float sustain, int release, int waveType)
{ 
    voice->setAttack(attack);
    voice->setDecay(decay);
    voice->setSustain(sustain);
    voice->setRelease(release);
    voice->setWaveType(waveType);
}

std::vector<float> chip::Module::advance(int numSamples)
{ 
    //the 0th elements are all added together, the 1st elements, 2nd, all the way to the 
	//nth elements and the result is returned (aka - move along the sound wave)
	std::vector<float>* mixedFinal = new std::vector<float>(numSamples, 0.0);
	std::vector<float>* temp = new std::vector<float>(numSamples, 0.0);
	
	// Flag indicating if the inactive polyvoices need to be deactivated.
	bool cleanupFlag = false;
	
	//for(int i = 0; i < NUM_POLYVOICES; i++)
	for(int i = 0; i < next; i++)
    {   
        if(((*polyvoices)[i].getState() == CLEANUP) && (next > 8))
        {
            cleanupFlag = true;
        }
        
        //for each IAudio in audioList, advance
        *temp = (*polyvoices)[i].advance(numSamples);
        for(int j = 0; j < numSamples; j++)
        {
            //sum each advanced IAudio to the master mixed vector
            (*mixedFinal)[j] = (*mixedFinal)[j] + (*temp)[j];
        }
        
    }
	
	temp->clear();
	delete temp;
	
	// Cleanup the polyvoices marked to be deactivated.
	if(cleanupFlag)
	{
	    cleanup();
	}
	
	return *mixedFinal; //the final, "synthesized" list
}

void chip::Module::activatePolyVoice(int note)
{
    int index = next;   
    
    // If there already exists a note in the active polyvoices, reset the
    // PolyVoice to attack. Otherwise activate the polyvoice at the next index.
    for(int i = 0; i < next; i++)
    {
        if((*polyvoices)[i].note == note)
        {
            index = i;
            break;
        }
        
        // If the note is greater than this note, insert this note before 
        // this index
        if((*polyvoices)[i].note > note)
        {
            shiftRightAt(i);
            index = i;
            break;
        }
    }
    
    (*polyvoices)[index].note = note;
    (*polyvoices)[index].phase = 0.0;
    (*polyvoices)[index].frequency = MtoF(note);
    (*polyvoices)[index].state = ATTACK;
    (*polyvoices)[index].setVoice(voice->getAttack(), 
                                  voice->getDecay(), 
                                  voice->getSustain(),
                                  voice->getRelease(),
                                  voice->getWaveType());
                                  
    if(index == next)
    {
        next++;
    }
}


void chip::Module::releasePolyVoice(int note)
{
    // Find a matching note and swap it with the last active polyvoice (next - 1).
    // By setting the to-be-deactivated polyvoice to the last active polyvoice and 
    // deactivating the last active polyvoice, we are swapping the two.
    for(int i = 0; i < next; i++)
    {
        if((*polyvoices)[i].note == note)
        {
            (*polyvoices)[i].releasePolyVoice();
            
            break;
        }
    }
}

void chip::Module::cleanup()
{
	// Take a snapshot of next so when we modify it in the loop, polyvoices 
    // aren't left hanging.
    int currentNext = next;
    
    // Find any note that is flagged for "clean up" and swap it with the
    // last active polyvoice (next - 1).
    for(int i = 0; i < currentNext; i++)
    {
        if((*polyvoices)[i].state == CLEANUP)
        {   
            shiftLeftAt(i);
            
            (*polyvoices)[next].state = OFF;
        }
    }
    
    printPolyVoices();
}

void chip::Module::shiftRightAt(int index)
{
    if(next == 0) return;

    int toSwap = next;
    
    /* If the index is 2, next is 4, and our array is:
     * 
     * [ 3 | 6 | 7 | 12 | - | - ]
     *
     * Then the result should look like:
     * 
     * [ 3 | 6 | - | 7 | 12 | - ]
     *
     * and next should be 5.
     */
    while(toSwap != index)
    {
        swap(toSwap, toSwap - 1);
        toSwap--;
    }
    
    next++;
}

void chip::Module::shiftLeftAt(int index)
{
    if(next == 0) return;
    
    int toSwap = index;
    
    /* If the index is 2, next is 5, and our array is:
     * 
     * [ 3 | 6 | - | 7 | 12 | - ]
     *
     * Then the result should look like:
     * 
     * [ 3 | 6 | 7 | 12 | - | - ]
     *
     * and next should be 4.
     */
    while((toSwap + 1) <= next)
    {
        swap(toSwap, toSwap + 1);
        toSwap++;
    }
    
    next--;
}

void chip::Module::swap(int a, int b)
{
    chip::PolyVoice* temp = new chip::PolyVoice();
    
    // temp = a
    temp->note = (*polyvoices)[a].note;
    temp->phase = (*polyvoices)[a].phase;
    temp->frequency = (*polyvoices)[a].frequency;
    temp->state = (*polyvoices)[a].getState();
    temp->setEnvmult((*polyvoices)[a].getEnvmult());
    temp->setEnvloc((*polyvoices)[a].getEnvloc());
    
    // a = b
    (*polyvoices)[a].note = (*polyvoices)[b].note;
    (*polyvoices)[a].phase = (*polyvoices)[b].phase;
    (*polyvoices)[a].frequency = (*polyvoices)[b].frequency;
    (*polyvoices)[a].state = (*polyvoices)[b].getState();
    (*polyvoices)[a].setEnvmult((*polyvoices)[b].getEnvmult());
    (*polyvoices)[a].setEnvloc((*polyvoices)[b].getEnvloc());
    
    // b = temp
    (*polyvoices)[b].note = temp->note;
    (*polyvoices)[b].phase = temp->phase;
    (*polyvoices)[b].frequency = temp->frequency;
    (*polyvoices)[b].state = temp->getState();
    (*polyvoices)[b].setEnvmult(temp->getEnvmult());
    (*polyvoices)[b].setEnvloc(temp->getEnvloc());
    
    delete temp;
}

//Midi Note to Frequency
float chip::Module::MtoF(int note){
	return pow(2,(((float)note - 69)/12.0))*440.0;
}

void chip::Module::printPolyVoices()
{
    for(int i = 0; i <= next + 4; i++)
    {
        std::cout << "polyvoice" << i << ": " << (*polyvoices)[i].note << "\n";
    }
    
    std::cout << "DONE\n\n";
}
