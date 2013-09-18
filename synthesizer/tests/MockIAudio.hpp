#include "../src/IAudio.hpp"
#include <vector>

class MockIAudio: public chip::IAudio 
{
    public:
        MockIAudio(float theValue, float theIncrement)
        {
            n = theValue;
            increment = theIncrement;
        }
        
        ~MockIAudio() {}
        
        std::vector<float> advance(int num)
        {
            std::vector<float>* arr = new std::vector<float>(0);
            int i = 0;
            for( i = 0; i < num; i++ )
            {
                arr->push_back(n);
                n = n + increment;
            }
            return *arr;
        }
            
    private:
        float n;
        float increment;
};
