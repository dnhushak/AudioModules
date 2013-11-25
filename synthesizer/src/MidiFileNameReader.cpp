#include "MidiFileNameReader.hpp"

void chip::MidiFileNameReader::readFile()
{
    this->midiFileList = new std::vector<std::string>();
    std::ifstream file(MIDI_FILE_NAMES);
    std::string line;
    
	if (file.is_open())
	{
	    	while (file.good())
		{
		    	getline(file,line);
			
            		std::string fileName;
        
            		std::stringstream ss(line);
            		ss >> fileName;
                
            		this->midiFileList->push_back(fileName);
		}
	    	file.close();
	}
	else
	{
		std::cout << "Unable to open file" << std::endl;
	}
    return;
}
