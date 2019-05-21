#include <AccelStepper.h>

// overall loop control
int RUN = 1;

// Define a stepper and the pins it will use
AccelStepper stepperX(AccelStepper::DRIVER, 8, 9);
AccelStepper stepperY(AccelStepper::DRIVER, 10, 11);

// define limit switch hardware inputs
int limitSwitchX = 5;
int limitSwitchY = 6;

// define well location at starting point
int well00_x = -100;
int well00_y = -100;

// define analog input
luminometerPin = A0;

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

  // example potential event loop
  /*
    if (user_input =='1')
  {
     StepForwardDefault();
  }
  else if(user_input =='2')
  {
    ReverseStepDefault();
  }
  else if(user_input =='3')
  {
    SmallStepMode();
  }
  else if(user_input =='4')
  {
    ForwardBackwardStep();
  }
  else
  {
    Serial.println("Invalid option entered.");
  }
  */

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

void moveGantry(stepper, position){

  // move stepper to the position
  stepper.move(position);

  // block until motor gets there
  while (stepper.distanceToGo() != 0){
    stepper.run();
  }
}


// function to move the scanner to a set point at well 1
void prepareForPlateScan(){
  
  // separately move xy to well 0 using preset coords
  moveGantry(stepperX, well00_x);
  moveGantry(stepperY, well00_y);

}


// function to read analog output from photoluminometer
int takeMeasurement(){
  
  // read the input pin
  val = analogRead(luminometerPin);  

  // return
  return val;

}


// function to repeatedly take measurements and write output to serial point
void rasterScan(stepper, direction, steps, stepSize){
  
  // directionality input checking
  if (direction == 0){
    direction = -1;
  }
  else if (direction == 1){
    direction = 1;
  }
  else{
    Serial.println("Illegal directionality set!");
  }

  // move gantry
  for(int i=0; i<steps; i++){
    moveGantry(stepper, stepSize * direction);
    meas = takeMeasurement();
    Serial.println(meas);
  }


}
