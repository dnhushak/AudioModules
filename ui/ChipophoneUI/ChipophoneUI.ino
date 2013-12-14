// constants won't change. Used here to 
// set pin numbers:

const int numModules = 5;

const int moduleStartPin = 25;

const int midiInPin = 18;
const int midiOutPin = 19;

/******************************************************************************/
/* LEDs                                                                    */
/******************************************************************************/

// Channel Selector LEDs
const int LEDRedSelectorPin = 25;
const int LEDYelSelectorPin = 26;
const int LEDGrnSelectorPin = 27;
const int LEDBluSelectorPin = 28;
const int LEDWhtSelectorPin = 29;

const int LEDArpeggioRedPin = 30;      // the number of the LED pin
const int LEDArpeggioGrnPin = 31;
const int LEDArpeggioBluPin = 32;

const int LEDGlissandoRedPin = 33;      // the number of the LED pin
const int LEDGlissandoGrnPin = 34;
const int LEDGlissandoBluPin = 35;

const int LEDPlayPin = 36;
const int LEDStopPin = 37;
const int LEDPausPin = 38;
const int LEDRecdPin = 39;


/******************************************************************************/
/* Buttons                                                                    */
/******************************************************************************/

const int buttonRedSelectorPin = A0;
const int buttonYelSelectorPin = A1;
const int buttonGrnSelectorPin = A2;
const int buttonBluSelectorPin = A3;
const int buttonWhtSelectorPin = A4;

const int buttonArpeggioPin = A5;
const int buttonGlissandoPin = A6;

int buttonStateArpeggioLast = HIGH;
int buttonStateGlissandoLast = HIGH;

const int buttonPlayPin = A7;
const int buttonPausPin = A8;
const int buttonStopPin = A9;
const int buttonRecdPin = A10;


/******************************************************************************/
/* Encoders                                                                   */
/******************************************************************************/

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

//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void sendMidi(int message, int data1, int data2) {
  Serial1.write(message);
  Serial1.write(data1);
  Serial1.write(data2);
}

void setup() {
  // set the digital pin as output:
  
  // LEDs
  pinMode(LEDArpeggioRedPin, OUTPUT);  
  pinMode(LEDArpeggioGrnPin, OUTPUT);     
  pinMode(LEDArpeggioBluPin, OUTPUT);
  pinMode(LEDGlissandoRedPin, OUTPUT);  
  pinMode(LEDGlissandoGrnPin, OUTPUT);     
  pinMode(LEDGlissandoBluPin, OUTPUT);

  pinMode(LEDRedSelectorPin, OUTPUT);     
  pinMode(LEDYelSelectorPin, OUTPUT);     
  pinMode(LEDGrnSelectorPin, OUTPUT);     
  pinMode(LEDBluSelectorPin, OUTPUT);     
  pinMode(LEDWhtSelectorPin, OUTPUT);     
  
  pinMode(LEDPlayPin, OUTPUT);     
  pinMode(LEDStopPin, OUTPUT);     
  pinMode(LEDPausPin, OUTPUT);     
  pinMode(LEDRecdPin, OUTPUT);     
  
  // Buttons 
  pinMode(buttonRedSelectorPin, INPUT);
  pinMode(buttonYelSelectorPin, INPUT);
  pinMode(buttonGrnSelectorPin, INPUT);
  pinMode(buttonBluSelectorPin, INPUT);
  pinMode(buttonWhtSelectorPin, INPUT);
  
  pinMode(buttonArpeggioPin, INPUT);
  pinMode(buttonGlissandoPin, INPUT);
  
  pinMode(buttonPlayPin, INPUT);
  pinMode(buttonPausPin, INPUT);
  pinMode(buttonStopPin, INPUT);
  pinMode(buttonRecdPin, INPUT);
  
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



void loop()
{
  int controlChange = 0xB0;
  int noteOn = 0x90;
  int note = 5;
  int arpeggio = 0x50;
  int toggle = 127;
  int value = 1;
  
  int currentButtonState = digitalRead(buttonArpeggioPin);
  
  if(currentButtonState == LOW &&
     buttonStateArpeggioLast == HIGH)
     {
        buttonStateArpeggioLast = currentButtonState;
        
        if(!arpeggioOn[0])
        {
            // Toggle LED
            digitalWrite(LEDArpeggioRedPin, LOW);
            digitalWrite(LEDArpeggioRedPin, LOW);
            digitalWrite(LEDArpeggioRedPin, LOW);
        }
        else
        {
            // Toggle LED
            digitalWrite(LEDArpeggioRedPin, HIGH);
            digitalWrite(LEDArpeggioRedPin, HIGH);
            digitalWrite(LEDArpeggioRedPin, HIGH);
        }
        // Toggle Arpeggio
        arpeggioOn[0] = !arpeggioOn[0];
        sendMidi(controlChange, arpeggio, toggle);
       
     }
  //sendMidi(noteOn, note, 0);
  //delay(100);
  
  // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
  /*state += readEncoder(encoderAPin);
  state %= 5;
  if (state<0){
    state = 4;
  }
  int i;
  for (i = 0; i < numModules; i++){
    moduleState[i] = digitalRead(modulePin[i]);

  }
  /*if (moduleState[5] == HIGH){
   
   state = 3;
   }*/
  /*for (i = 0; i< numModules; i++){
    if (moduleState[i] == HIGH){
      state = i;
    } 
  }
  int rgb[3] = {
    LOW, LOW, LOW        };
  stateDecode(state, rgb);
  digitalWrite(redPin, !rgb[0]);
  digitalWrite(grnPin, !rgb[1]);
  digitalWrite(bluPin, !rgb[2]);
  */
}









