// constants won't change. Used here to 
// set pin numbers:
const int redPin = 8;      // the number of the LED pin
const int grnPin = 9;
const int bluPin = 10;
const int numModules = 5;
const int moduleStartPin = 2;
const int encoderAPin = 11;

// Variables will change:
int state;
int moduleState[numModules];
int modulePin[numModules]; //State switch pins

int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;


void setup() {
  // set the digital pin as output:
  pinMode(redPin, OUTPUT);  
  pinMode(grnPin, OUTPUT);     
  pinMode(bluPin, OUTPUT);    
  pinMode(encoderAPin, INPUT);
  pinMode(encoderAPin + 1, INPUT);
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
    LOW, LOW, LOW      };
  stateDecode(state, rgb);
  digitalWrite(redPin, !rgb[0]);
  digitalWrite(grnPin, !rgb[1]);
  digitalWrite(bluPin, !rgb[2]);
}








