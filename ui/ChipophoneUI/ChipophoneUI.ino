// constants won't change. Used here to 
// set pin numbers:

const int numModules = 5;

const int moduleStartPin = 25;

const int midiInPin = 18;
const int midiOutPin = 19;


/******************************************************************************/
/* Channel Selection Buttons and LEDs                                                                    */
/******************************************************************************/

const int redSelectorButton = A1;
const int redSelectorLED = 39;

const int yelSelectorButton = A0;
const int yelSelectorLED = 38;

const int grnSelectorButton = A3;
const int grnSelectorLED = 37;

const int bluSelectorButton = A2;
const int bluSelectorLED = 36;

const int whtSelectorButton = A5;
const int whtSelectorLED = 35;

int channelSelectorButtons[] = {
  redSelectorButton, 
  yelSelectorButton, 
  grnSelectorButton,
  bluSelectorButton,
  whtSelectorButton };
  
int channelSelectorLEDs[] = {
  redSelectorLED, 
  yelSelectorLED, 
  grnSelectorLED,
  bluSelectorLED,
  whtSelectorLED };

/******************************************************************************/
/* Edit Channel Buttons and LEDs                                                                    */
/******************************************************************************/

const int arpeggioButton = A4;
const int arpeggioRedLED = 34;      // the number of the LED pin
const int arpeggioGrnLED = 33;
const int arpeggioBluLED = 32;

int lastArpeggioState = HIGH;

const int glissButton = A7;
const int glissRedLED = 31;      // the number of the LED pin
const int glissGrnLED = 30;
const int glissBluLED = 29;

int lastGlissState = HIGH;


/******************************************************************************/
/* Songbox Buttons and LEDs                                                                    */
/******************************************************************************/
/*
const int playButton = A9;
const int playLED = 26;

const int pausButton = A8;
const int pausLED = 25;

const int stopButton = A6;
const int stopLED = 27;

const int recdButton = A10;
const int recdLED = 28;
*/

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

int arpeggioOn[numModules];
int glissandoOn[numModules];

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

//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void sendMidi(int message, int data1, int data2) {
  Serial1.write(message);
  Serial1.write(data1);
  Serial1.write(data2);
}

void setup() {
  // set the digital pin as output:
  
  // Select Channel to Edit
  for(int i=0; i<numModules; i++)
  {
     pinMode(channelSelectorButtons[i], INPUT);
     digitalWrite(channelSelectorButtons[i], HIGH); // RELEASED
     
     pinMode(channelSelectorLEDs[i], OUTPUT);
     digitalWrite(channelSelectorLEDs[i], HIGH); // OFF
  }
  
  pinMode(arpeggioButton, INPUT);
  digitalWrite(arpeggioButton, HIGH);
  
  // Initialize the module to 0
  digitalWrite(channelSelectorLEDs[0], LOW); //on
  currentlySelectedModule = 0;
  
  /*
  // Arpeggio
  pinMode(buttonArpeggioPin, INPUT);
  pinMode(LEDArpeggioRedPin, OUTPUT);  
  pinMode(LEDArpeggioGrnPin, OUTPUT);     
  pinMode(LEDArpeggioBluPin, OUTPUT);
  
  // Glissando
  pinMode(buttonGlissandoPin, INPUT);
  pinMode(LEDGlissandoRedPin, OUTPUT);  
  pinMode(LEDGlissandoGrnPin, OUTPUT);     
  pinMode(LEDGlissandoBluPin, OUTPUT);

  // Songbox 
  pinMode(buttonPlayPin, INPUT);
  pinMode(buttonPausPin, INPUT);
  pinMode(buttonStopPin, INPUT);
  pinMode(buttonRecdPin, INPUT);
  
  pinMode(LEDPlayPin, OUTPUT);     
  pinMode(LEDStopPin, OUTPUT);     
  pinMode(LEDPausPin, OUTPUT);     
  pinMode(LEDRecdPin, OUTPUT);     
  */
  // MIDI I/O
  pinMode(midiInPin, INPUT);
  pinMode(midiOutPin, OUTPUT);
  
  // Encoders
  pinMode(encoderAChannelVolumeAPin, INPUT);
  pinMode(encoderAChannelVolumeBPin, INPUT);
  pinMode(encoderBArpeggioAPin, INPUT);
  pinMode(encoderBArpeggioBPin, INPUT);
  pinMode(encoderCGlissandoAPin, INPUT);
  pinMode(encoderCGlissandoBPin, INPUT);
  pinMode(encoderDTempoAPin, INPUT);
  pinMode(encoderDTempoBPin, INPUT);
  
  int i;
  for (i = 0; i< numModules; i++){
      modulePin[i] = moduleStartPin + i;
      pinMode(modulePin[i], INPUT); 
    
      /* Encoders */
      encoderAPos[i] = 0;
      pinMode(encoderAPos[i], INPUT);
      encoderBPos[i] = 0;
      pinMode(encoderBPos[i], INPUT);
      encoderCPos[i] = 0;
      pinMode(encoderCPos[i], INPUT);
      encoderDPos[i] = 0;
      pinMode(encoderDPos[i], INPUT);
      
      arpeggioOn[i] = 0;
      pinMode(arpeggioOn[i], INPUT);
      glissandoOn[i] = 0;
      pinMode(glissandoOn[i], INPUT);
  }
  
  Serial1.begin(31250);
  Serial.begin(9600);
  Serial.println("Start");
  
  //sendMidi(controlChange, arpeggio, toggle);
  

}

void stateDecode(int state, int *rgb){
  switch (state){
  case 0:
    rgb[0] = HIGH;
    break;
  case 1:
    rgb[0] = rgb[1] = HIGH;
    break;
  case 2:
    rgb[1] = HIGH;
    break;
  case 3:
    rgb[2] = HIGH;
    break;
  case 4:
    rgb[0] = rgb[1] = rgb[2] = HIGH;
    break;
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
    
    for(i=0; i<numModules; i++)
    {
       int button = channelSelectorButtons[i];
       int led = channelSelectorLEDs[i];
       
       //if(digitalRead(button) == LOW) //pressed
       if(isPressed(button))
       {
         Serial.print("LED on ");
         Serial.println(led);
         
         // Turn off the previous module LED
         digitalWrite(channelSelectorLEDs[currentlySelectedModule], HIGH);
         
         // Update the currently selected module
         currentlySelectedModule = i;
         
         // Turn on the currently selected module LED
         digitalWrite(led, LOW); //on
         break;
       }  
    } 
}

void toggleArpeggio()
{ 
  if(isPressed(arpeggioButton) && (lastArpeggioState != LOW))
  {
    Serial.println("Arpeggio toggle");
    lastArpeggioState = LOW;
  }
  else if(!isPressed(arpeggioButton))
  {                                                                                                                                                                                             ");
    lastArpeggioState = HIGH; 
  }
}

void loop()
{
  // Check status of  Channel Selector buttons
  selectChannelToEdit();
  toggleArpeggio();
}









