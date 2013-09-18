#include "../src/Mixer.hpp"
#include "MockIAudio.hpp"
#include "Assert.hpp"
#include <vector>

chip::Mixer* mixer;

void TestAdvanceWithZeroAdditions();
void TestAdvanceWithOneAddition1Sample();
void TestAdvanceWithOneAddition2Samples();
void TestAdvanceWithTwoAdditions2Samples();
void TestAdvancingRepeatedly();

int main(void)
{
    std::cout << "\n--- Testing Mixer ---\n";
    
    // Create the mixer.
    mixer = new chip::Mixer();
    
    // Run test methods.
    TestAdvanceWithZeroAdditions();
    TestAdvanceWithOneAddition1Sample();
    TestAdvanceWithOneAddition2Samples();
    TestAdvanceWithTwoAdditions2Samples();
    TestAdvancingRepeatedly();

    return 0;
}

void TestAdvanceWithZeroAdditions()
{
    Assert::isNotNULL( mixer, "Mixer should not be null.\n" );
    std::vector<float>* expected = new std::vector<float>(1); // array size 0
    std::vector<float> actual = mixer->advance(1);
    
    Assert::areEqual( *expected, actual, "No errors should occur when advancing empty mixer.\n" );
}

void TestAdvanceWithOneAddition1Sample()
{
    mixer = new chip::Mixer();
    
    MockIAudio* mock = new MockIAudio(5, 0); // Start at 5 and increment by 0
    mixer->addObjects((chip::IAudio*)mock);
    
    std::vector<float>* expected = new std::vector<float>(0);
    expected->push_back(5);
    
    std::vector<float> actualFromMixer = mixer->advance(1); // Only advance one
    Assert::areEqual( *expected, actualFromMixer, "TestAdvanceWithOneAddition1Sample -> mixer\n" );
}

void TestAdvanceWithOneAddition2Samples()
{
    mixer = new chip::Mixer();
    
    MockIAudio* mock = new MockIAudio(1, 10); // Start at 1 and increment by 10
    mixer->addObjects((chip::IAudio*)mock);
    
    std::vector<float>* expected = new std::vector<float>(0);
    expected->push_back(1);
    expected->push_back(11);
    
    std::vector<float> actualFromMixer = mixer->advance(2); // Only advance one
    Assert::areEqual( *expected, actualFromMixer, "TestAdvanceWithOneAddition1Sample -> mixer\n" );
}

void TestAdvanceWithTwoAdditions2Samples()
{
    mixer = new chip::Mixer();
    
    MockIAudio* mock1 = new MockIAudio(2, 2); // Start at 2 and increment by 2. [2,4,6,8,10,...]
    mixer->addObjects((chip::IAudio*)mock1);
    
    MockIAudio* mock2 = new MockIAudio(10, -1); // Start at 10 and increment by -1. [10,9,8,7,...]
    mixer->addObjects((chip::IAudio*)mock2);
    
    std::vector<float>* expected = new std::vector<float>(0);
    expected->push_back(12); // First elements added together. 2 + 10 = 12
    expected->push_back(13); // Second elements added together. 4 + 9 = 13
    
    std::vector<float> actualFromMixer = mixer->advance(2); // Only advance one
    Assert::areEqual( *expected, actualFromMixer, "TestAdvanceWithTwoAdditions1Sample -> mixer\n" );
}

void TestAdvancingRepeatedly()
{
    mixer = new chip::Mixer();
    
    MockIAudio* mock1 = new MockIAudio(2, 2); // Start at 2 and increment by 2. [2,4,6,8,10,...]
    mixer->addObjects((chip::IAudio*)mock1);
    
    MockIAudio* mock2 = new MockIAudio(10, -1); // Start at 10 and increment by -1. [10,9,8,7,...]
    mixer->addObjects((chip::IAudio*)mock2);
    
    std::vector<float>* expected = new std::vector<float>(0);
    expected->push_back(14); // Third elements added together. 6 + 8 = 14
    expected->push_back(15); // Fourth elements added together. 8 + 7 = 15
    
    mixer->advance(2);
    std::vector<float> actualFromMixer = mixer->advance(2); // Advance an additional 2
    Assert::areEqual( *expected, actualFromMixer, "TestAdvanceWithTwoAdditions1Sample -> mixer\n" );      
}
