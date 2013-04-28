#include <mixer.hpp>
	
Mixer::Mixer(vector<IAudio> elements){	//constructor
	audioList = new vector<IAudio>();
}

vector<float> Mixer::advance(int numSamples){
	//the 0th elements are all added together, the 1st elements, 2nd, all the way to the 
	//nth elements and the result is returned (aka - move along the sound wave)
	vector<float> mixedFinal = new vector<float>();
	vector<float> temp = new vector<float>(numSamples);
	for(int i = 0; i<audioList.size(); i++){ //for each IAudio in audioList, advance
		temp_advance = audioList[i].advance(numSamples);
		for(int j = 0; j<numSamples; j++){ //sum each advanced IAudio to the master mixed vector
			mixedFinal[j] = mixedFinal[j] + temp_advance[j];
		}
	}
	return mixedFinal; //the final, "synthesized" list
}

void Mixer::addObjects(IAudio theThingToAdd){
	audioList.push_back(theThingToAdd);
}
	
