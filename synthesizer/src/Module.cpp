#include <Module.hpp>

Module::Module(stuff){ //constructor

}

vector<float> Module::advance(int numSamples){
	return mixer.advance(numSamples);
}