#include <Module.hpp>

Module::Module(stuff){ //constructor

}

std::vector<float> Module::advance(int numSamples){
	return mixer.advance(numSamples);
}
