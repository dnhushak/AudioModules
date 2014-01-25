#include "MIDI.h"

// constants won't change. Used here to 
// set pin numbers:

//Glissando control number and toggle number (both CC numbers)
#define GLISSCCNUM  (0b00000101)
#define GLISSTOGGLE (0b01000001)

//Arpeggio control number and toggle number (both CC numbers)
#define ARPEGCCNUM  (0b01010001)
#define ARPEGTOGGLE (0b01010000)

const int numModules = 5;
const int moduleStartPin = 2;
const int midiInPin = 18;
const int midiOutPin = 19;


/******************************************************************************/
/* Channel Selection Buttons and LEDs                                         */
/******************************************************************************/

const int OFF = -1;
const int RED = 0;
const int YEL = 1;
const int GRN = 2;
const int BLU = 3;
const int WHT = 4;

const int redSelectorButton = A1;
const int yelSelectorButton = A0;
const int grnSelectorButton = A3;
const int bluSelectorButton = A2;
const int whtSelectorButton = A5;

int channelSelectorButtons[] = {
  redSelectorButton, 
  yelSelectorButton, 
  grnSelectorButton,
  bluSelectorButton,
  whtSelectorButton };

const int redSelectorLED = 39;
const int yelSelectorLED = 38;
const int grnSelectorLED = 37;
const int bluSelectorLED = 36;
const int whtSelectorLED = 35;

int channelSelectorLEDs[] = {
  redSelectorLED, 
  yelSelectorLED, 
  grnSelectorLED,
  bluSelectorLED,
  whtSelectorLED };

/******************************************************************************/
/* Edit Channel Buttons and LEDs                                              */
/******************************************************************************/

const int arpeggioButton = A4;
const int arpeggioRedLED = 34;      // the number of the LED pin
const int arpeggioGrnLED = 33;
const int arpeggioBluLED = 32;

int arpeggioLEDs[] = {
  arpeggioRedLED, arpeggioGrnLED, arpeggioBluLED };

//int *lastArpeggioState;
int lastArpeggioState = HIGH;
boolean arpeggioOn[numModules];

const int glissButton = A7;
const int glissRedLED = 31;      // the number of the LED pin
const int glissGrnLED = 30;
const int glissBluLED = 29;

int glissLEDs[] = {
  glissRedLED, glissGrnLED, glissBluLED };

int lastGlissState = HIGH;
boolean glissOn[numModules];

/******************************************************************************/
/* Songbox Buttons and LEDs                                                                    */
/******************************************************************************/

const int playButton = A9;
const int playLED = 26;

const int pausButton = A8;
const int pausLED = 25;

const int stopButton = A6;
const int stopLED = 27;

const int recdButton = A10;
const int recdLED = 28;


/******************************************************************************/
/* Encoders                                                                   */
/******************************************************************************/
// TODO Test correct pins for encoders
const int encoderAChannelVolumeAPin = 2;
const int encoderAChannelVolumeBPin = 3;
const int encoderBArpeggioAPin = 4;
const int encoderBArpeggioBPin = 5;
const int encoderCGlissandoAPin = 6;
const int encoderCGlissandoBPin = 7;
const int encoderDTempoAPin = 8;
const int encoderDTempoBPin = 9;


/******************************************************************************/
/* End constants                                                              */
/******************************************************************************/

// Variables will change:
int currentlySelectedModule = 0;
int state;
int moduleState[numModules];
int modulePin[numModules]; //State switch pins

int encoderAPos[numModules];
int encoderBPos[numModules];
int encoderCPos[numModules];
int encoderDPos[numModules];

int encoderAChannelVolumeAPinLast = LOW;
int encoderAChannelVolumeBPinLast = LOW;
int encoderBArpeggioAPinLast = LOW;
int encoderBArpeggioBPinLast = LOW;
int encoderCGlissandoAPinLast = LOW;
int encoderCGlissandoBPinLast = LOW;
int encoderDTempoAPinLast = LOW;
int encoderDTempoBPinLast = LOW;

int n = LOW;

boolean isPressed(int button)
{
  return digitalRead(button) == LOW; 
}

void turnOn(int pin)
{
  digitalWrite(pin, LOW);
}

void turnOff(int pin)
{
  digitalWrite(pin, HIGH); 
}

//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void sendMidi(byte message, byte data1, byte data2) {
  Serial1.write(message);
  Serial1.write(data1);
  Serial1.write(data2);
}

void setup() {

  int i;


  // Select Channel to Edit
  for(i=0; i<numModules; i++)
  {
    pinMode(channelSelectorButtons[i], INPUT);
    digitalWrite(channelSelectorButtons[i], HIGH); // RELEASED

    pinMode(channelSelectorLEDs[i], OUTPUT);
    digitalWrite(channelSelectorLEDs[i], HIGH); // OFF
  }

  // Initialize the module to 0
  digitalWrite(channelSelectorLEDs[0], LOW); //on
  currentlySelectedModule = 0;

  // Arpeggio
  pinMode(arpeggioButton, INPUT);
  //Gives button a pullup resistor
  digitalWrite(arpeggioButton, HIGH);
  pinMode(arpeggioRedLED, OUTPUT); 
  turnOff(arpeggioRedLED);
  pinMode(arpeggioGrnLED, OUTPUT); 
  turnOff(arpeggioGrnLED);
  pinMode(arpeggioBluLED, OUTPUT);
  turnOff(arpeggioBluLED);

  for(i = 0; i < numModules; i++)
  {
    // Initially all modules have arpeggio off
    arpeggioOn[i] = false;
  }

  // Glissando
  pinMode(glissButton, INPUT);
  //Gives button a pullup resistor
  digitalWrite(glissButton, HIGH);
  pinMode(glissRedLED, OUTPUT); 
  turnOff(glissRedLED);
  pinMode(glissGrnLED, OUTPUT); 
  turnOff(glissGrnLED);
  pinMode(glissBluLED, OUTPUT);
  turnOff(glissBluLED);

  for(i = 0; i < numModules; i++)
  {
    // Initially all modules have arpeggio off
    glissOn[i] = false;
  }

  // Songbox 
  pinMode(playButton, INPUT);
  digitalWrite(playButton, HIGH);
  pinMode(pausButton, INPUT);
  digitalWrite(pausButton, HIGH);
  pinMode(stopButton, INPUT);
  digitalWrite(stopButton, HIGH);
  pinMode(recdButton, INPUT);
  digitalWrite(recdButton, HIGH);


  pinMode(playLED, OUTPUT);
  pinMode(pausLED, OUTPUT);
  pinMode(stopLED, OUTPUT);
  pinMode(recdLED, OUTPUT);

  // MIDI I/O
  pinMode(midiInPin, INPUT);
  pinMode(midiOutPin, OUTPUT);

  // Encoders
  /*
  pinMode(encoderAChannelVolumeAPin, INPUT);
   pinMode(encoderAChannelVolumeBPin, INPUT);
   pinMode(encoderBArpeggioAPin, INPUT);
   pinMode(encoderBArpeggioBPin, INPUT);
   pinMode(encoderCGlissandoAPin, INPUT);
   pinMode(encoderCGlissandoBPin, INPUT);
   pinMode(encoderDTempoAPin, INPUT);
   pinMode(encoderDTempoBPin, INPUT);
   
   for (i = 0; i< numModules; i++){
   modulePin[i] = moduleStartPin + i;
   pinMode(modulePin[i], INPUT); 
   
   // Encoders
   encoderAPos[i] = 0;
   pinMode(encoderAPos[i], INPUT);
   encoderBPos[i] = 0;
   pinMode(encoderBPos[i], INPUT);
   encoderCPos[i] = 0;
   pinMode(encoderCPos[i], INPUT);
   encoderDPos[i] = 0;
   pinMode(encoderDPos[i], INPUT);
   }
   */

  Serial1.begin(31250);
  Serial.begin(9600);
  Serial.println("Start");

  demoSequence();

  //sendMidi(controlChange, arpeggio, toggle);

}

void changeLEDColor(int *rgbPin, int desiredColor )
{
  int R = 0;
  int G = 1;
  int B = 2;

  switch (desiredColor)
  {  
  case RED:
    turnOn(rgbPin[R]);
    turnOff(rgbPin[G]);
    turnOff(rgbPin[B]);
    break;

  case YEL:
    turnOn(rgbPin[R]);
    turnOn(rgbPin[G]);
    turnOff(rgbPin[B]);
    break;

  case GRN:
    turnOff(rgbPin[R]);
    turnOn(rgbPin[G]);
    turnOff(rgbPin[B]);
    break;

  case BLU:
    turnOff(rgbPin[R]);
    turnOff(rgbPin[G]);
    turnOn(rgbPin[B]);
    break;

  case WHT:
    turnOn(rgbPin[R]);
    turnOn(rgbPin[G]);
    turnOn(rgbPin[B]);
    break;

  default:
    turnOff(rgbPin[R]);
    turnOff(rgbPin[G]);
    turnOff(rgbPin[B]);
  }
}

int readEncoder(int encoderPin){
  int ret = 0;
  /*int encoderPinA = encoderPin;
   int encoderPinB = encoderPin + 1;
   n = digitalRead(encoderPinA);
   if ((encoder0PinALast == LOW) && (n == HIGH)) {
   if (digitalRead(encoderPinB) == LOW) {
   ret = -1;
   } 
   else {
   ret = 1;
   }
   } 
   encoder0PinALast = n;
   if(ret != 0){
   Serial.print(ret);
   Serial.print("\n");
   }
   */
  return ret;
}


void selectChannelToEdit()
{
  int i=0;

  for(i = 0; i < numModules; i++)
  {
    int button = channelSelectorButtons[i];
    int led = channelSelectorLEDs[i];

    if(isPressed(button))
    {
      turnOff(channelSelectorLEDs[currentlySelectedModule]);
      currentlySelectedModule = i;

      // Turn on the currently selected module LED
      turnOn(led);

      // Update the channel editting button LEDs
      if(arpeggioOn[currentlySelectedModule])
      {
        changeLEDColor(arpeggioLEDs, currentlySelectedModule);
      }  
      else
      {
        changeLEDColor(arpeggioLEDs, OFF);
      }

      if(glissOn[currentlySelectedModule])
      {
        changeLEDColor(glissLEDs, currentlySelectedModule);
      }  
      else
      {
        changeLEDColor(glissLEDs, OFF);
      }
      break;
    }  
  } 
}

void updateChannelProps(int button, int* lastState, int* rgbPins, boolean* moduleState, byte data1)
{ 
  if((*lastState == HIGH) && isPressed(button))
  {
    *lastState = LOW;
    delay(5);

    byte message = (CC << 4) + currentlySelectedModule;
    byte data2;
    // Toggle the property
    if(moduleState[currentlySelectedModule])
    {
      // Turn all the things off
      changeLEDColor(rgbPins, OFF);
      moduleState[currentlySelectedModule] = false;

      data2 = MIDIOFF;

    }  
    else
    {
      // Set led buttons to the color of the selected channel.
      changeLEDColor(rgbPins, currentlySelectedModule);
      moduleState[currentlySelectedModule] = true;     
      data2 = MIDION; 
    }

    sendMidi(message,data1,data2);
  }
  else if(!isPressed(button))
  {
    *lastState = HIGH; 
  }
}

void loop()
{
  // Check status of  Channel Selector buttons
  selectChannelToEdit();

  if(isPressed(playButton)){
    turnOn(playLED); 
  }

  if(isPressed(stopButton)){
    turnOn(stopLED);
    turnOff(playLED); 
  }
  else{
    turnOff(stopLED);
  }

  if(isPressed(redSelectorButton) && isPressed(bluSelectorButton) && isPressed(yelSelectorButton) && isPressed(grnSelectorButton) && isPressed(whtSelectorButton)){
    mushroom();
    demoSequence(); 
  }

  // Argeggio and glissando have similar logic
  updateChannelProps(arpeggioButton, &lastArpeggioState, arpeggioLEDs, arpeggioOn, ARPEGTOGGLE);
  updateChannelProps(glissButton, &lastGlissState, glissLEDs, glissOn, GLISSTOGGLE);
}

void demoSequence(){
  int numLEDs = 15;
  int LEDs[] = {
    redSelectorLED, yelSelectorLED, grnSelectorLED, bluSelectorLED, whtSelectorLED, arpeggioRedLED, arpeggioGrnLED, arpeggioBluLED, glissRedLED, glissGrnLED, glissBluLED, playLED, pausLED, stopLED, recdLED          };

  int j;
  int i;
  for (j=0; j<4; j++){
    for (i=0; i<numLEDs; i++){
      turnOn(LEDs[i]); 
    }
    delay(150);
    for (i=0; i<numLEDs; i++){
      turnOff(LEDs[i]); 
    }
    delay(150);
  }
  for (i=0; i<numLEDs; i++){
    turnOn(LEDs[i]);
    delay(100);
    turnOff(LEDs[i]);
  }



}


void mushroom(){
  byte message = (NOTEON << 4);
  byte data1;
  int i;
  for(i=0;i<26;i++){
    switch(i){
    case 0:
      data1 = 60;
      break;
    case 1:
      data1 = 53;
      break;
    case 2:
      data1 = 60;
      break;
    case 3:
      data1 = 64;
      break;
    case 4:
      data1 = 67;
      break;
    case 5:
      data1 = 72;
      break;
    case 6:
      data1 = 67;
      break;
    case 7:
      data1 = 56;
      break; 
    case 8:
      data1 = 60;
      break;
    case 9:
      data1 = 63;
      break;
    case 10:
      data1 = 68;
      break;
    case 11:
      data1 = 63;
      break;
    case 12:
      data1 = 68;
      break;
    case 13:
      data1 = 72;
      break;
    case 14:
      data1 = 75;
      break;
    case 15:
      data1 = 80;
      break;
    case 16:
      data1 = 75;
      break;
    case 17:
      data1 = 58;
      break;
    case 18:
      data1 = 62;
      break;
    case 19:
      data1 = 65;
      break;
    case 20:
      data1 = 70;
      break;
    case 21:
      data1 = 65;
      break;
    case 22:
      data1 = 70;
      break;
    case 23:
      data1 = 74;
      break; 
    case 24:
      data1 = 77;
      break;
    case 25:
      data1 = 82;
      break;
    } 
    sendMidi(message,data1,MIDION);  
    delay(35);           
    sendMidi(message,data1,MIDIOFF); 

  }
}













