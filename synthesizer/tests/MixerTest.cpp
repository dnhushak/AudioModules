#include "../src/Mixer.hpp"
#include "MockIAudio.hpp"
#include "Assert.hpp"
#include <vector>
#define TEST_BUFFER_SIZE 10
#define TEST_ADVANCE 10
#define TEST_BUFFER_SIZE_2 20
#define TEST_ADVANCE_2 20

chip::Mixer * mixer;

void TestAdvanceWithZeroAdditions(int, int);
void TestAdvanceWithOneAddition(int, int);
void TestAdvanceWithTwoAdditions(int, int);
void TestTwoAdvancesWithTwoAdditions(int, int);

int main(void) {
	std::cout << "\n--- Testing Mixer ---\n";

	// Create the mixer with buffer size of 10.
	mixer = new chip::Mixer(TEST_BUFFER_SIZE);

	// Run test methods
	std::cout << "=============Zero IAudio Devices=============\n";
	TestAdvanceWithZeroAdditions(TEST_ADVANCE, TEST_BUFFER_SIZE);
	std::cout << "=============  1 IAudio Device  =============\n";
	TestAdvanceWithOneAddition(TEST_ADVANCE, TEST_BUFFER_SIZE);
	mixer->removeObject(0);
	std::cout << "============= Two IAudio Devices=============\n";
	TestAdvanceWithTwoAdditions(TEST_ADVANCE, TEST_BUFFER_SIZE);
	mixer->removeAllObjects();
	mixer->resizeBuffer(TEST_BUFFER_SIZE_2);
	std::cout << "=============   Resized Buffer  =============\n";
	TestAdvanceWithTwoAdditions(TEST_ADVANCE_2, TEST_BUFFER_SIZE_2);
	mixer->removeAllObjects();
	std::cout << "=============    Two Advances   =============\n";
	TestTwoAdvancesWithTwoAdditions(TEST_ADVANCE_2, TEST_BUFFER_SIZE_2);

	return 0;
}

void TestAdvanceWithZeroAdditions(int advance, int bufferSize) {
	Assert::isNotNULL(mixer, "Mixer should not be null.\n");
	float * expected = new float[bufferSize];
	for (int i = 0; i < bufferSize; i++) {
		expected[i] = 0.0;
	}
	float * actual = mixer->advance(advance);

	for (int i = 0; i < advance; i++) {
		Assert::areEqual(expected[i], actual[i],
				"TestAdvanceWithNoAddtions -> mixer\n");
	}
}

void TestAdvanceWithOneAddition(int advance, int bufferSize) {

	chip::MockIAudio * mock = new chip::MockIAudio(bufferSize); // Start at 5 and increment by 0
	mixer->addAudioDevice((chip::AudioDevice*) mock);

	float * expected = new float[bufferSize];
	for (int i = 0; i < bufferSize; i++) {
		expected[i] = -i;
		mock->buffer[i] = -i;
	}

	float * actual = mixer->advance(advance); // Only advance one
	for (int i = 0; i < advance; i++) {
		Assert::areEqual(expected[i], actual[i],
				"TestAdvanceWithOneAddition -> mixer\n");
	}
}

void TestAdvanceWithTwoAdditions(int advance, int bufferSize) {

	chip::MockIAudio * mock1 = new chip::MockIAudio(bufferSize);
	chip::MockIAudio * mock2 = new chip::MockIAudio(bufferSize);
	mixer->addAudioDevice((chip::AudioDevice*) mock1);
	mixer->addAudioDevice((chip::AudioDevice*) mock2);

	float * expected = new float[bufferSize];
	for (int i = 0; i < bufferSize; i++) {
		expected[i] = 2 * i;
		mock1->buffer[i] = i;
		mock2->buffer[i] = i;
	}

	float * actual = mixer->advance(advance); // Only advance one
	for (int i = 0; i < advance; i++) {
		Assert::areEqual(expected[i], actual[i],
				"TestAdvanceWithTwoAddition -> mixer\n");
	}

}

void TestTwoAdvancesWithTwoAdditions(int advance, int bufferSize) {

	chip::MockIAudio * mock1 = new chip::MockIAudio(bufferSize);
	chip::MockIAudio * mock2 = new chip::MockIAudio(bufferSize);
	mixer->addAudioDevice((chip::AudioDevice*) mock1);
	mixer->addAudioDevice((chip::AudioDevice*) mock2);

	float * expected = new float[bufferSize];
	for (int i = 0; i < bufferSize; i++) {
		expected[i] = 2 * i;
		mock1->buffer[i] = i;
		mock2->buffer[i] = i;
	}

	//Advance twice, since same input buffers, should be the same
	mixer->advance(advance);
	float * actual = mixer->advance(advance);
	for (int i = 0; i < advance; i++) {
		Assert::areEqual(expected[i], actual[i],
				"TestAdvanceWithTwoAddition -> mixer\n");
	}

}
