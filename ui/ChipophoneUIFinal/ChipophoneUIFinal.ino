//Pin Assignments
const int encoderAPin = 2;
const int encoderBPin = 4;
const int encoderCPin = 6;
const int encoderDPin = 8;


//Encoder state variables
int encoderALast = LOW;
int encoderBLast = LOW;
int encoderCLast = LOW;
int encoderDLast = LOW;



void setup(){
  //I/O Assigments
  Serial.begin (9600);
  pinMode(encoderAPin, INPUT);
  pinMode(encoderAPin + 1, INPUT);
  pinMode(encoderBPin, INPUT);
  pinMode(encoderBPin + 1, INPUT);
  pinMode(encoderCPin, INPUT);
  pinMode(encoderCPin + 1, INPUT);
  pinMode(encoderDPin, INPUT);
  pinMode(encoderDPin + 1, INPUT);

}


//Read Encoder function
int readEncoder(int encoderPin){
  int ret = 0;
  int encoderPin1 = encoderPin;
  int encoderPin2 = encoderPin + 1;
  
  int n;
  //Read current state of first pin
  n = digitalRead(encoderPin1);
  if ((encoderBLast== LOW) && (n == HIGH)) {
    if (digitalRead(encoderPin2) == LOW) {
      ret = -1;
    } 
    else {
      ret = 1;
    }
  } 
  encoderALast = n;
  if(ret != 0){
    Serial.print(ret);
    Serial.print("\n");
  }
  return ret;
}

void loop(){
 readEncoder(encoderBPin); 
}

