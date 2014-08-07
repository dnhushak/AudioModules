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
#include "Limiter.hpp"
#include "AudioDevice.hpp"
#include <pthread.h>

// Main
int main(int argc, char *argv[]) {
	modules::Gain * gain = new modules::Gain();
	printf("Gain 1:  %d\n", gain->getDevID());
	modules::Gain * gain2 = new modules::Gain();
	printf("Gain 2:  %d\n", gain2->getDevID());
	modules::Gain * gain3 = new modules::Gain();
	printf("Gain 3:  %d\n", gain3->getDevID());
	//delete gain3;
	modules::Gain * gain4 = new modules::Gain();
	printf("Gain 4:  %d\n", gain4->getDevID());

	modules::Mixer * mixer = new modules::Mixer();
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
