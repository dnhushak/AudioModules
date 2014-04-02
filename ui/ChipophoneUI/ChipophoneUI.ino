#include "Pinout.h"
#include "Button.h"
#include "ChannelSelector.h"
#include "LED.h"
#include "RGBLED.h"
#include "ArduinoMIDIHandler.h"

const int REDBUTTON  = A1;
const int YELBUTTON  = A0;
const int GRNBUTTON  = A3;
const int BLUBUTTON  = A2;
const int WHTBUTTON  = A5;

const int REDLED = 39;
const int YELLED = 38;
const int GRNLED = 37;
const int BLULED = 36;
const int WHTLED = 35;

const int ARPBUTTON = A4;
const int ARPLEDRED = 34;
const int ARPLEDGRN =  33;
const int ARPLEDBLU = 32;

const int GLISSBUTTON = A7;
const int GLISSLEDRED = 31;
const int GLISSLEDGRN = 30;
const int GLISSLEDBLU = 29;


int numChannels = 5;
UI::Button * buttonA;
UI::ChannelSelector * channelSel;
UI::LED ** channelLED;
UI::RGBLED * arpLED;
UI::RGBLED * glissLED;

synth::ArduinoMIDIHandler * AMHandler;

int channelSelPins[5] = {REDBUTTON, YELBUTTON, GRNBUTTON, BLUBUTTON, WHTBUTTON};
int channelSelLEDs[5] = {REDLED, YELLED, GRNLED, BLULED, WHTLED};
int currentState;
int lastState;


void setup() {
  // Start serial comm for debugging
  Serial.begin(9600);
  Serial.println("Start");

  // Initialize the LED's
  channelLED = (UI::LED **) malloc(sizeof(UI::LED *) * numChannels);

  for (int i = 0; i < numChannels; i++) {
    channelLED[i] = new UI::LED(channelSelLEDs[i]);
    channelLED[i]->begin();
    channelLED[i]->on();
  }
  
  arpLED = new UI::RGBLED(ARPLEDRED, ARPLEDGRN, ARPLEDBLU);
  glissLED = new UI::RGBLED(GLISSLEDRED, GLISSLEDGRN, GLISSLEDBLU);
  arpLED->begin();
  glissLED->begin();
  arpLED->on();
  glissLED->on();



  // Initialize the buttons
  channelSel = new UI::ChannelSelector(channelSelPins, numChannels);
  channelSel->begin();
  buttonA = new UI::Button(A10);
  buttonA->begin();

  // Initialize MIDI
  AMHandler = new synth::ArduinoMIDIHandler(&Serial1);
  AMHandler->begin();

}

void loop() {
  channelSel->poll();
  currentState = channelSel->getState();
  if (currentState != lastState) {
    lastState = currentState;
    Serial.println(currentState, DEC);
    synth::MIDIMessage * msg = new synth::MIDIMessage;
    msg->channel = currentState;
    msg->statusType = synth::CC;
    msg->data1 = 100;
    msg->data2 = 25;
    AMHandler->writeMIDI(msg);
    for (int i = 0; i < numChannels; i++) {
      channelLED[i]->off();
      if (i == currentState) {
        channelLED[i]->on();
      }
    }
  }



}


