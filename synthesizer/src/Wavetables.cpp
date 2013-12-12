#include "Wavetables.hpp"
#include <iostream>


using namespace chip;

bool Wavetables::instanceFlag;
chip::Wavetables *Wavetables::single;

float Wavetables::getSample(int waveType, int phase) 
{
    if (waveType > NUM_WAVES || phase > TABLE_SIZE )
    {
        std::cerr << "Warning:(Wavetables.cpp) Array out of bounds in the wavetable.\n";
        waveType = waveType % NUM_WAVES;
        phase = phase % TABLE_SIZE;
    }
    
    if(waveType == NOISE)
    {
        return rand() % (16384 * 2) - 16384;
    }

    return table[waveType][phase];
}

// Vibrato is basically a multiplier that ranges from -0.005 to 0.005.
// This multiplier adds a percentage of the frequency to itself.
float Wavetables::getVibrato(int i)
{
    if(i > 359)
    {
        return 0.0;
    }
    
    return vibrato[i];
}

//Generate Waves
void Wavetables::wavetableGen(){

    int i;

    //Divide TABLE_SIZE into four regions
    int quarter = TABLE_SIZE/4;
    int half = TABLE_SIZE/2;
    int three_fourths = 3*quarter;

    //Cycle through the entirety of TABLE_SIZE and generate triangle, and square waves
    //The triangle wave statement takes the current index of the for loop, casts it to a float (to do division), and scales it to do the correct math in triangle wave generation
	for (i = 0; i < TABLE_SIZE; i++)
	{    
		table[SAWTOOTH][i] = -16384 + (((float)i/TABLE_SIZE) * 16384);
	    
	    table[NOISE][i] = rand() % (16384 * 2) - 16384;
	    
		//First half of the wave
		if ( i < half )
		{
			table[SQUARE][i] = -16384;
			table[PULSE][i] = -16384;
			table[TRIANGLE][i] = -16384 + (((float)i/quarter) * 16384);
		}
		//Third quarter of the wave
		else if ( i < three_fourths )
		{
			table[SQUARE][i] = 16383;
			table[PULSE][i] = -16384;
			table[TRIANGLE][i] = 16383 - ((((float)i-half)/quarter) * 16384);
		}
		//Fourth quarter of the wave
		else 
		{
			table[SQUARE][i] = 16383;
			table[PULSE][i] = 16384;
			table[TRIANGLE][i] = 16383 - ((((float)i-half)/quarter) * 16384);
		}
		
		// Create the vibrato wave table
		for(int i = 0; i < 360; i++)
		{
		    vibrato[i] = 0.005 * sin(i * (PI / 180.0));
		}
		
	}
}
