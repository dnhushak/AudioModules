#include <stdio.h>
#include <stdlib.h>
#include "Songbox.hpp"
#include "SongboxMidiParser.hpp"

int main(int argc, char *argv[])
{
	//Grab midi device ID from arguments
	int devID;

	if (argc!=2){
		printf("Enter MIDI Device ID: ./Songbox #\n");
		return -1;
	}
	else{
		devID = atoi(argv[1]);
	}
    
    // Connect to the MIDI stream and start reading
    chip::SongboxMidiParser* midiParser = new chip::SongboxMidiParser();
    midiParser->connectToMIDIStream(devID);
    
    // Block the front end until someone hits enter
    // We are getting audio callbacks while this is happening
    std::cin.ignore(255, '\n'); // NOTE: I don't know if we still need this line in the songbox
}

