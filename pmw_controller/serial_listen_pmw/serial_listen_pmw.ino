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

  /*
  if (in > 0){
    Serial.println(in);
    analogWrite(ledPin, in);
  }
  else if (in < 0){
    Serial.println(in);
    analogWrite(ledPin, 0);
  }
  */
  if (in == 1){
    int stimIntensity = 255;
    beginStimulusProtocol(stimIntensity);
  }
 
}

void beginStimulusProtocol(int stimIntensity){
  
  delay(10000);
  analogWrite(ledPin, stimIntensity);
  delay(3000);
  analogWrite(ledPin, 0);
  delay(30000);
  analogWrite(ledPin, stimIntensity);
  delay(2000);
  analogWrite(ledPin, 0);
  delay(20000);
  analogWrite(ledPin, stimIntensity);
  delay(10000);
  analogWrite(ledPin, 0);
  delay(20000);
  analogWrite(ledPin, stimIntensity);
  delay(5000);
  analogWrite(ledPin, 0);
  delay(10000);
  analogWrite(ledPin, stimIntensity);
  delay(1000);
  analogWrite(ledPin, 0);

}
