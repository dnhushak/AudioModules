#pragma once
namespace chip
{

    class PolyVoice
    {
        public:
            PolyVoice();
            ~Polyvoice();
        
        private:
            int note;
			float phase;
            float frequency;
            int isActive;
    };
    
}
