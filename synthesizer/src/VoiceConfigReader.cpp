#include "VoiceConfigReader.hpp"

void chip::VoiceConfigReader::readFile()
{
    this->voices = new std::vector<chip::Voice>();
    std::ifstream file(VOICE_CONFIG_FILE);
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
        
            std::stringstream ss(line);
            ss >> attack >> decay >> sustain >> release >> waveTypeString;
            
            waveType = this->convertWaveType(waveTypeString);
            Voice* newVoice = new Voice(
                attack, decay, sustain, release, waveType);
                
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
