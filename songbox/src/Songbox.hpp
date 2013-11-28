#pragma once

#include "MidiFileNameReader.hpp"

namespace chip
{
class Songbox
{
    public:
        MidiFileNameReader* fileNameReader;
	int defaultSong; //song is set to 0 by default (link.mid)
    
        Songbox()
        {
        }

	//plays the song at the given index, plays default song if index is invalid
	void playSongAtIndex(int index);
        
    private:
        
};  
}
