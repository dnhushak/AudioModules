#include <Module.hpp>

Module::Module()
{ 
	//constructor
}

std::vector<float> Module::advance(int numSamples)
{
	return mixer.advance(numSamples);
}
