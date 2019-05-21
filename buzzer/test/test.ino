int ledPin = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
}

void loop() {

  // output 5v. analog (PMW)?
  int dutyCycle = 127;
  analogWrite(ledPin, dutyCycle);
  delay(1000);
  analogWrite(ledPin, 0);
  delay(1000);

  // digital write?
  //digitalWrite(ledPin, HIGH);
  //delay(1000);
  //digitalWrite(ledPin, LOW);
  //delay(1000);

}
