// constants won't change. Used here to 
// set pin numbers:

const int numModules = 5;
const int moduleStartPin = 25;

/******************************************************************************/
/* Buttons                                                                    */
/******************************************************************************/

const int LEDArpeggioRedPin = 30;      // the number of the LED pin
const int LEDArpeggioGrnPin = 31;
const int LEDArpeggioBluPin = 32;

const int LEDGlissandoRedPin = 33;      // the number of the LED pin
const int LEDGlissandoGrnPin = 34;
const int LEDGlissandoBluPin = 35;

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

int encoderAPos = 0;
int encoderBPos = 0;
int encoderCPos = 0;
int encoderDPos = 0;

int encoderAChannelVolumeAPinLast = LOW;
int encoderAChannelVolumeBPinLast = LOW;
int encoderBArpeggioAPinLast = LOW;
int encoderBArpeggioBPinLast = LOW;
int encoderCGlissandoAPinLast = LOW;
int encoderCGlissandoBPinLast = LOW;
int encoderDTempoAPinLast = LOW;
int encoderDTempoBPinLast = LOW;

int n = LOW;


void setup() {
  // set the digital pin as output:
  pinMode(LEDArpeggioRedPin, OUTPUT);  
  pinMode(LEDArpeggioGrnPin, OUTPUT);     
  pinMode(LEDArpeggioBluPin, OUTPUT);
  pinMode(LEDGlissandoRedPin, OUTPUT);  
  pinMode(LEDGlissandoGrnPin, OUTPUT);     
  pinMode(LEDGlissandoBluPin, OUTPUT);

  /* Encoders */
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
  }

  Serial.begin (9600);
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
  int encoderPinA = encoderPin;
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
  return ret;
}

void loop()
{

  // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
  state += readEncoder(encoderAPin);
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
  for (i = 0; i< numModules; i++){
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
}









