int incomingByte = 0;
int ledPin = 6;

void setup() {

  //Declaring LED pin as output
  pinMode(ledPin, OUTPUT);

  // turn off the LED
  digitalWrite(ledPin, HIGH);

  // begin listening
  Serial.begin(9600);
}

void loop() {
  
  // read the incoming byte:
  incomingByte = Serial.read();



  // say what you got:
  // ASCII printable characters: 49 means number 1
  if(incomingByte == 49) { 
    analogWrite(ledPin, 0);
  } 
  // ASCII printable characters: 48 means number 0
  else if(incomingByte == 48) { 
    analogWrite(ledPin, 255);
  }

  if(incomingByte != -1){
    Serial.print("Done!");
  }
 
}
