#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, 8, 9);

int pos = 3600;
int limitSwitch = 2;
int switchState = 0;

void setup()
{  
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(3000);
  pinMode(limitSwitch, INPUT_PULLUP);

  // initialize serial interface
  Serial.begin(9600);
}


void loop()
{
  // read limit switch circuit.
  switchState = digitalRead(limitSwitch);
  if (!(switchState)){
    Serial.println("Limit switch engaged!");
    stepper.stop();
    //stepper.moveTo(stepper.currentPosition());
  }
  else if (stepper.distanceToGo() == 0)
  {
    delay(500);
    pos = -pos;
    stepper.moveTo(pos);
  }
  stepper.run();
}
