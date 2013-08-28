#pragma once
namespace chip
{

    class PolyVoice
    {
        public:
            PolyVoice();
            ~PolyVoice();
        
        private:
            int note;
			float phase;
            float frequency;
            int isActive;
    };
    
}
