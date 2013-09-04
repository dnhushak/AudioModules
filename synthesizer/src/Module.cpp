#include <Module.hpp>

chip::Module::Module()
{ 
	//constructor
}

std::vector<float> chip::Module::advance(int numSamples)
{
	return mixer.advance(numSamples);
}
