#include "../src/Wavetables.hpp"
#include "Assert.hpp"

synth::Wavetables* wavetable;

void TestOnlyOneInstance();
void TestSquareWave();

int main(void)
{
    std::cout << "\n--- Testing Wavetables ---\n";
    
    // Create the first and only instance of the Wavetables.
    wavetable = synth::Wavetables::getInstance();
    
    TestOnlyOneInstance();
    TestSquareWave();
    
    return 0;
}

void TestOnlyOneInstance()
{
    Assert::isNotNULL( wavetable, "Wavetables should not be null.\n" );
    
    synth::Wavetables* second = synth::Wavetables::getInstance();
    Assert::areEqual( wavetable, second, "Only one instance should be created.\n" );
}

void TestSquareWave()
{
    int i = 0;
    
    float squareWaveSample = wavetable->getSample(SQUARE, 0);
    Assert::areEqual( -16384, squareWaveSample, "Square wave sample: 0\n" );
    
    squareWaveSample = wavetable->getSample(SQUARE, 5);
    Assert::areEqual( -16384, squareWaveSample, "Square wave sample: 5\n" );
    
    squareWaveSample = wavetable->getSample(SQUARE, 10);
    Assert::areEqual( 16383, squareWaveSample, "Square wave sample: 10\n" );
    
    squareWaveSample = wavetable->getSample(SQUARE, 15);
    Assert::areEqual( 16383, squareWaveSample, "Square wave sample: 15\n" );
    
    squareWaveSample = wavetable->getSample(SQUARE, 17);
    Assert::areEqual( -16384, squareWaveSample, "Square wave sample: 17 (out of bounds)\n" );
    
    squareWaveSample = wavetable->getSample(SQUARE, 29);
    Assert::areEqual( 16383, squareWaveSample, "Square wave sample: 29 (out of bounds)\n" );
       
}
