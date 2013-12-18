int currentLED = 25;

void setup(){
  int i;
  for (i=25; i<40; i++){
    pinMode (i,OUTPUT);
    digitalWrite(i, HIGH);
  }
  Serial.begin(9600);
  Serial.println("Start");
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  digitalWrite(A0,HIGH);
  digitalWrite(A1,HIGH);
  digitalWrite(A2,HIGH);
  digitalWrite(A3,HIGH);
  digitalWrite(A4,HIGH);
  digitalWrite(A5,HIGH);
  digitalWrite(A6,HIGH);
  digitalWrite(A7,HIGH);
  digitalWrite(A8,HIGH);
  digitalWrite(A9,HIGH);
  digitalWrite(A10,HIGH);
}

void loop(){
  //int i;
  //  for (i=25; i<40; i++){
  //    digitalWrite(i,LOW);
  //    delay(500);
  //  }
  //  for (i=25; i<40; i++){
  //    digitalWrite(i,HIGH);
  //    delay(500);
  //  }
  
  //BRITTANY 
  int i;
  int a[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10};
  
  for (i=0; i<11; i++){
    
    int led = 28;
    
    if(digitalRead(a[i]) == LOW)
    {
      Serial.print("LED on ");
      Serial.println(led);
      digitalWrite(led, LOW); //on
    }  
    else
    {  
      digitalWrite(led, HIGH); //off
    }
  }
  
  
  
  // END BRITTANY
  /*
  if(digitalRead(A1) == LOW){
    digitalWrite(25,LOW);
    Serial.print("LED 25\n");
  }
  else
  { 
    digitalWrite(25,HIGH);
  }


  if(digitalRead(A0) == LOW){
    Serial.print("LED 27\n");
    digitalWrite(27,LOW);
  }
  else
  { 
    digitalWrite(27,HIGH);
  }

  if(digitalRead(A2) == LOW){
    Serial.print("LED 28\n");
    digitalWrite(28,LOW);
  }
  else
  { 
    digitalWrite(28,HIGH);
  }

  if(digitalRead(A3) == LOW){
    Serial.print("LED 29\n");
    digitalWrite(29,LOW);
  }
  else
  { 
    digitalWrite(29,HIGH);
  }  
  
  if(digitalRead(A4) == LOW){
    Serial.print("LED 30\n");
    digitalWrite(30,LOW);
  }    
  else
  { 
    digitalWrite(30,HIGH);
  }
  
  
  if(digitalRead(A5) == LOW){
    Serial.print("LED 31\n");
    digitalWrite(31,LOW);
  }    
  else
  { 
    digitalWrite(31,HIGH);
  }
  
  
  if(digitalRead(A6) == LOW){
    Serial.print("LED 32\n");
    digitalWrite(32,LOW);
  }    
  else
  { 
    digitalWrite(32,HIGH);
  }
  
  if(digitalRead(A7) == LOW){
    Serial.print("LED 33\n");

    digitalWrite(33,LOW);
  } 
  else
  { 
    digitalWrite(33,HIGH);
  }
  
  
  if(digitalRead(A8) == LOW){
    Serial.print("LED 34\n");

    digitalWrite(34,LOW);
  }
  else
  { 
    digitalWrite(34,HIGH);
  }
  
  if(digitalRead(A9) == LOW){
    Serial.print("LED 35\n");

    digitalWrite(35,LOW);
  }
  else
  { 
    digitalWrite(35,HIGH);
  }

  if(digitalRead(A10) == LOW){
    Serial.print("LED 36\n");

    digitalWrite(36,LOW);
  }
  else
  { 
    digitalWrite(37,HIGH);
  } */

}







