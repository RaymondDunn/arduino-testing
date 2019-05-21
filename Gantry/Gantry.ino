#include <AccelStepper.h>

// overall loop control
int RUN = 1;

// Define a stepper and the pins it will use
AccelStepper stepperX(AccelStepper::DRIVER, 8, 9);
AccelStepper stepperY(AccelStepper::DRIVER, 10, 11);

// define limit switch hardware inputs
int limitSwitchX = 5;
int limitSwitchY = 6;

void setup()
{  
  // set motor params
  stepperX.setMaxSpeed(1000);
  stepperY.setMaxSpeed(1000);
  stepperX.setAcceleration(3000);
  stepperY.setAcceleration(3000);

  // set limit switch params
  pinMode(limitSwitchX, INPUT_PULLUP);
  pinMode(limitSwitchY, INPUT_PULLUP);

  // initialize serial interface
  Serial.begin(9600);
}


void loop()
{

  if (RUN){
    setZeroPosition();
  }
  RUN = 0;

}

void setZeroPosition(){

  int increment = 10;
  int setX = 0;
  int setY = 0;

  // move x in one direction
  while (!(setX && setY)){

    // continue moving x until set
    if (!(setX)){
      if (digitalRead(limitSwitchX)){
        stepperX.move(increment);
      }
      else{
        stepperX.stop();
        setX = 1;
      }
    }
    
    // continue moving y until set
    if (!(setY)){
      if (digitalRead(limitSwitchY)){
        stepperY.move(increment);
      }
      else{
        stepperY.stop();
        setY = 1;
      }
    }
    
    // advance motors
    stepperX.run();
    stepperY.run();
  }  

  // now that we've moved to the zero position, set that as 0
  stepperX.setCurrentPosition(0);
  stepperY.setCurrentPosition(0);
  
}
