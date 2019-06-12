int incomingByte = 0;
int ledPin = 6;

void setup() {

  //Declaring LED pin as output
  pinMode(ledPin, OUTPUT);

  // turn off the LED
  digitalWrite(ledPin, LOW);

  // begin listening
  Serial.begin(9600);
}

void loop() {
  
  // read the incoming byte:
  long in = Serial.parseInt();
  if (in > 0){
    Serial.println(in);
    analogWrite(ledPin, in);
  }
  else if (in < 0){
    Serial.println(in);
    analogWrite(ledPin, 0);
  }
 
}
