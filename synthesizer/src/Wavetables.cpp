#include "Wavetables.hpp"

//Power of wavetable size (wavetable size = 2 ^ POWER)
#define POWER (4)
#define TABLE_SIZE  (1<<POWER)
#define PHASESCALE ((1<<17)-1)
#define NUM_SECONDS (1)

using namespace chip;

enum
{
	SQUARE, PULSE, TRIANGLE, SAWTOOTH, NOISE
}WaveEnum;

Wavetables::Wavetables(int waveTypeEnum)
{
	waveType = waveTypeEnum
	//initialize 5x16 table
	float table[5][16];
}

float Wavetables::getSample(WaveEnum waveType, float phase, int numSamples) 
{
	
    return 0;    //TODO lern2C++
}

//Generate Waves
void wavetablegen(void){

    int i;

    //Divide TABLE_SIZE into four regions
    int quarter = TABLE_SIZE/4;
    int half = TABLE_SIZE/2;
    int three_fourths = 3*quarter;

    //Cycle through the entirety of TABLE_SIZE and generate triangle, and square waves
    //The triangle wave statement takes the current index of the for loop, casts it to a float (to do division), and scales it to do the correct math in triangle wave generation
	for (i=0;i<TABLE_SIZE;i++){
		//First half of the wave
    		if (i<half){
    			tri[i] = -16384 + (((float)i/quarter) * 16384);
    			sq1[i] = -16384;
    			sq2[i] = -16384;
    		}
    		//Third quarter of the wave
    		else if (i<three_fourths){
    			tri[i] = 16383 - ((((float)i-half)/quarter) * 16384);
    			sq1[i] = 16383;
    			sq2[i] = -16384;
    		}
    		//Fourth quarter of the wave
    		else {
    			tri[i] = 16384 - ((((float)i-half)/quarter) * 16384);
    			sq1[i] = 16383;
    			sq2[i] = 16383;
    		}
    		nse[i] = rand() % 16384 - 16384;
    	}
}