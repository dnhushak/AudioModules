#include "ChannelSelector.h"

namespace UI {

	ChannelSelector::ChannelSelector(int * initPins, int initNumChannels) {
		// Number of channels
		numChannels = initNumChannels;
		// Point the pins to the initialized array of pins
		buttons = (Button **) malloc(sizeof(Button *) * numChannels);

		// Initialize all of the buttons
		for (int i = 0; i < numChannels; i++) {
			buttons[i] = new Button(initPins[i]);
		}

		// Initial state of 0
		state = 0;
	}

// Calls begin on all of the buttons
	void ChannelSelector::begin() {
		// Begin all buttons
		for (int i = 0; i < numChannels; i++) {
			buttons[i]->begin();
		}
	}

	void ChannelSelector::poll() {
		for (int i = 0; i < numChannels; i++) {
			// Poll each of the buttons
			buttons[i]->poll();

			// Check its state; if it is pressed, then set the state to the button number
			// Note if two buttons are pressed, highest number takes precedence
			if (buttons[i]->isPressed()) {
				state = i;
			}
		}
	}

	void ChannelSelector::pollDebounce() {
		for (int i = 0; i < numChannels; i++) {
			// Poll each of the buttons using the debounce method
			buttons[i]->pollDebounce();

			// Check its state; if it is pressed, then set the state to the button number
			// Note if two buttons are pressed, highest number takes precedence
			if (buttons[i]->isPressed()) {
				state = i;
			}
		}

	}

	int ChannelSelector::getState() {
		return state;
	}

	ChannelSelector::~ChannelSelector() {
		for (int i = 0; i < numChannels; i++) {
			delete buttons[i];
		}
		free(buttons);
	}

}
