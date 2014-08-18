#include "PortAudioHandler.hpp"
#include "PortMIDIHandler.hpp"
#include "Mixer.hpp"
#include "ChannelFilter.hpp"
#include "Wavetable.hpp"
#include "Oscillator.hpp"
#include "MessagePrinter.hpp"
#include "Module.hpp"
#include "Voice.hpp"
#include "Gain.hpp"
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include "Clipper.hpp"
#include "AudioDevice.hpp"
#include <pthread.h>

// Main
int main(int argc, char *argv[]) {
	audio::Gain * gain = new audio::Gain();
	printf("Gain 1:  %d\n", gain->getDevID());
	audio::Gain * gain2 = new audio::Gain();
	printf("Gain 2:  %d\n", gain2->getDevID());
	audio::Gain * gain3 = new audio::Gain();
	printf("Gain 3:  %d\n", gain3->getDevID());
	//delete gain3;
	audio::Gain * gain4 = new audio::Gain();
	printf("Gain 4:  %d\n", gain4->getDevID());

	audio::Mixer * mixer = new audio::Mixer();
	printf("Mixer:   %d\n", mixer->getDevID());
	mixer->addDevice(gain);
	mixer->addDevice(gain2);
	mixer->addDevice(gain3);
//	printf("Mixer 1: %d\n",mixer->getDeviceByLoc(0)->getDevID());
//	printf("Mixer 2: %d\n",mixer->getDeviceByLoc(1)->getDevID());
//	printf("Mixer 3: %d\n",mixer->getDeviceByLoc(2)->getDevID());
//	printf("Mixer 6: %d\n",mixer->getDeviceByLoc(5)->getDevID());
//	printf("Mixer-2: %d\n",mixer->getDeviceByLoc(-3)->getDevID());
	mixer->addDevice(1,gain4);
//	printf("Mixer 2: %d\n",mixer->getDeviceByLoc(1)->getDevID());
//	printf("Mixer 3: %d\n",mixer->getDeviceByLoc(2)->getDevID());

}
