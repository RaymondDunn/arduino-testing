// define pin
int pin = 6;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  // control var
  int RUN = 0;
  int dutyCycle = 30;
  
  // duration of pulse
  int duration = 500;
  int waittime = 1500;
  // loop!
  if(RUN){
    analogWrite(pin, dutyCycle);
    delay(duration);
    analogWrite(pin, 0);
    delay(waittime);
  }

 
}
