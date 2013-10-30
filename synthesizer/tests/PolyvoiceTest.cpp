#include "../src/PolyVoice.hpp"
#include "Assert.hpp"
#include <vector>

chip::PolyVoice* polyVoice;

// Test methods
void TestStepSize();
void TestAdvanceSimple();
void TestAdvancingRepeatedly();

// Helper methods
void SetPolyvoice(int note, unsigned short phase, int frequency, int isActive);

// Run the tests
int main(void) {
	std::cout << "\n--- Testing PolyVoice ---\n";

	// Create the mixer.
	polyVoice = new chip::PolyVoice();

	// Run test methods.
	TestStepSize();
	TestAdvanceSimple();
	TestAdvancingRepeatedly();

	return 0;
}

void TestStepSize() {
	polyVoice->frequency = 2100;

	unsigned int expected = (2100 * PHASESCALE) / SAMPLE_RATE;
	unsigned int actual = polyVoice->stepsize();

	std::cout << "TODO Is this the answer we want???? ";
	Assert::areEqual(expected, actual, "TestStepSize.\n");
}

void TestAdvanceSimple() {
	Assert::isNotNULL(polyVoice, "PolyVoice should not be null.\n");

	SetPolyvoice(80, 0, 2100, 1);

	std::vector<float>* expected = new std::vector<float>(0);
	expected->push_back(-16384);
	expected->push_back(-16384);

	std::vector<float> actual = polyVoice->advance(2);

	Assert::areNotEqual(80, polyVoice->phase,
			"TestAdvanceSimple: Phase should advance\n");

	Assert::areEqual(*expected, actual, "TestAdvanceSimple.\n");
}

void TestAdvancingRepeatedly() {
	Assert::isNotNULL(polyVoice, "PolyVoice should not be null.\n");

	SetPolyvoice(80, 0, 2100, 1);

	int expectedPhase = 80;
	polyVoice->advance(1);
	Assert::areNotEqual(expectedPhase, polyVoice->phase,
			"TestAdvancingRepeatedly: Phase should advance\n");

	expectedPhase = polyVoice->phase;
	polyVoice->advance(1);
	Assert::areNotEqual(expectedPhase, polyVoice->phase,
			"TestAdvancingRepeatedly: Phase should advance\n");

	expectedPhase = polyVoice->phase;
	std::vector<float> actual = polyVoice->advance(1);
	Assert::areNotEqual(expectedPhase, polyVoice->phase,
			"TestAdvancingRepeatedly: Phase should advance\n");

	std::vector<float>* expected = new std::vector<float>(0);
	expected->push_back(16384);

	Assert::areEqual(*expected, actual, "TestAdvancingRepeatedly.\n");
}

void SetPolyvoice(int note, unsigned short phase, int frequency, int isActive) {
	polyVoice->note = note;
	polyVoice->phase = phase;
	polyVoice->frequency = frequency;
	polyVoice->state = isActive;
}

