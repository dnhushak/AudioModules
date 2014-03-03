#include "VoiceConfigReader.hpp"

bool chip::VoiceConfigReader::instanceFlag = false;
chip::VoiceConfigReader *chip::VoiceConfigReader::single = NULL;

void chip::VoiceConfigReader::readFile()
{

	char * filepath;

	filepath = getenv("CHIPCONFIG");
	if (filepath != NULL) {
		printf("Reading from config file: %s\n",filepath);
	}
	else{
		printf("Please define the CHIPCONFIG environment variable to point to the absolute path of the configuration text file\n");
		exit(1);
	}
    this->voices = new std::vector<chip::Voice>();
    std::ifstream file(filepath);
    std::string line;
    
	if (file.is_open())
	{
	    while (file.good())
		{
	    	getline(file,line);
		
	        int attack, decay, release;
    		float sustain;
    		std::string waveTypeString;
    		int waveType;
    		float vibratoAmplitude;
    		int vibratoPeriod;
    		int vibratoDelay;

    		std::stringstream ss(line);
    		ss >> attack >> decay >> sustain >> release >> waveTypeString >> 
    		   vibratoAmplitude >> vibratoPeriod >> vibratoDelay;
    
    		waveType = this->convertWaveType(waveTypeString);
    		Voice* newVoice = new Voice(
        		attack, decay, sustain, release, waveType, 
        		vibratoAmplitude, vibratoPeriod, vibratoDelay);
        
    		this->voices->push_back(*newVoice);
		}
	    file.close();
	}
	else
	{
		std::cout << "Unable to open file" << std::endl;
	}
    return;
}

int chip::VoiceConfigReader::convertWaveType(std::string waveTypeString)
{
    int waveType;

    if (waveTypeString == "SQUARE")         waveType = SQUARE;
    else if (waveTypeString == "PULSE")     waveType = PULSE;
    else if (waveTypeString == "TRIANGLE")  waveType = TRIANGLE;
    else if (waveTypeString == "SAWTOOTH")  waveType = SAWTOOTH;
    else if (waveTypeString == "NOISE")     waveType = NOISE;
    else
    {
        waveType = SQUARE; // default
    }
    
    return waveType;
}
