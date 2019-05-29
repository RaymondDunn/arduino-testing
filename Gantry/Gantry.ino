#include <AccelStepper.h>

// overall loop control
int RUN = 1;
int INPUT_SIZE = 30;

// Define a stepper and the pins it will use
AccelStepper stepperX(AccelStepper::DRIVER, 8, 9);
AccelStepper stepperY(AccelStepper::DRIVER, 10, 11);

// define limit switch hardware inputs
int limitSwitchX = 5;
int limitSwitchY = 6;

// define well location at starting point
int well00_x = 0;
int well00_y = 0;

// define analog input
int luminometerPin = A0;

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

  // initialize gantry to idle
  idleGantry();
}


void loop()
{

  while(Serial.available()){
    String user_input = Serial.readString();
    //Serial.println("Input: " + user_input);

    // main event loop
    // upshift and downshift to conserve power
    activateGantry();
    parseSerialInput(user_input);
    idleGantry();
  }
  
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

void moveGantry(String stepper, int pos){

  if (stepper == "x"){
    stepperX.move(pos); 
  }
  else if (stepper == "y"){
    stepperY.move(pos);
  }
  else{
    Serial.println("Invalid directionality for stepper!");
    return;
  }

  // block until motor gets there
  while ((stepperX.distanceToGo()) != 0 || (stepperY.distanceToGo() != 0)){
    stepperX.run();
    stepperY.run();
  }
}


// function to move the scanner to a set point at well 1
void prepareForPlateScan(){
  
  // separately move xy to well 0 using preset coords
  moveGantry("x", well00_x);
  moveGantry("y", well00_y);

}


// function to read analog output from photoluminometer
int takeMeasurement(){
  
  // read the input pin
  int val = analogRead(luminometerPin);  

  // return
  return val;

}


// function to repeatedly take measurements and write output to serial point
void rasterScan(String stepper, int steps, int stepSize){

  // initialize measurement variables
  int val = takeMeasurement();    // this is also necessary to clear photoluminometer of dark current
  int gain = getPhotoluminometerGain(); 
  long x_pos = getGantryPosition("x");
  long y_pos = getGantryPosition("y"); 
  
  // move gantry
  for(int i=0; i<steps-1; i++){

    // get data
    val = takeMeasurement();
    gain = getPhotoluminometerGain(); 
    x_pos = getGantryPosition("x");
    y_pos = getGantryPosition("y");
    sendMeasurementSerial(val, gain, x_pos, y_pos);

    // move gantry
    moveGantry(stepper, stepSize);
  }

  // do one last scan for last well
  val = takeMeasurement();
  gain = getPhotoluminometerGain(); 
  x_pos = getGantryPosition("x");
  y_pos = getGantryPosition("y");
  sendMeasurementSerial(val, gain, x_pos, y_pos);
}

// function to parse input string
void parseSerialInput(String user_input){

  // input is formatted: fxn,param1,param2
  // no spaces etc
  String funcname = getValue(user_input, ',', 0);

  // main parse switch statement
  if (funcname == "moveGantry"){

    // process input
    String stepperToMove = getValue(user_input, ',', 1);
    int pos = getValue(user_input, ',', 2).toInt();

    // execute movement
    Serial.println("Moving gantry " + stepperToMove + " by " +  String(pos));
    moveGantry(stepperToMove, pos);
  }
  else if (funcname == "setZeroPosition"){
    Serial.println("Setting zero position...");
    setZeroPosition();
  }
  else if (funcname == "prepareForPlateScan"){
    Serial.println("Preparing for plate scan...");
    setZeroPosition();
    moveGantry("x", well00_x);
    moveGantry("y", well00_y);
  }
  else if (funcname == "raster2DScan"){

    // make sure we are zerod
    Serial.println("Preparing for plate scan...");
    setZeroPosition();
    moveGantry("x", well00_x);
    moveGantry("y", well00_y);

    // get values and run
    int stepsX = getValue(user_input, ',', 1).toInt();
    int stepsY = getValue(user_input, ',', 2).toInt();
    int stepSize = getValue(user_input, ',', 3).toInt();
    Serial.println("Beginning raster2D:  " + String(stepsX) + " by " + String(stepsY) + " with stepSize: " + String(stepSize));
    raster2DScan(stepsX, stepsY, stepSize);
  }
  else if (funcname == "setSpeed"){
    int spd = getValue(user_input, ',', 1).toInt();
    Serial.println("Setting maximum speed of gantry to " + String(spd));
    setGantrySpeed(spd);
  }
  else if (funcname == "rasterScan"){
    String stepperToMove = getValue(user_input, ',', 1);
    int steps = getValue(user_input, ',', 2).toInt();
    int stepSize = getValue(user_input, ',', 3).toInt(); 
    Serial.println("Beginning raster: " + stepperToMove + "; steps: " + String(steps) + "; stepSize: " + String(stepSize));
    rasterScan(stepperToMove, steps, stepSize);
    Serial.println("Done raster!");
  }
  else if (funcname == "takeMeasurement"){
    int val = takeMeasurement();
    int gain = getPhotoluminometerGain();
    long x_pos = getGantryPosition("x");
    long y_pos = getGantryPosition("y");
    sendMeasurementSerial(val, gain, x_pos, y_pos);
    //Serial.println(val);
  }
  else if (funcname == "getGantryPosition"){
    String stepper = getValue(user_input, ',', 1);
    Serial.println("Getting gantry position at " + stepper);
    long val = getGantryPosition(stepper);
    Serial.println(val);

  }
  else if (funcname == "setGantryAcceleration"){
    Serial.println("Changing gantry acceleration!");
    int acceleration = getValue(user_input, ',', 1).toInt();
    setGantryAcceleration(acceleration);
  }
  else{
    Serial.println("Unrecognized input!");
  }
  
}

// helper function to get a value from a string
String getValue(String data, char separator, int index){
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


// helper function to change speed
void setGantrySpeed(int gantrySpeed){
  stepperX.setMaxSpeed(gantrySpeed);
  stepperY.setMaxSpeed(gantrySpeed);
}

// helper function to get current position
long getGantryPosition(String stepper){

  long pos = -9999;
  if (stepper == "x"){
    pos = stepperX.currentPosition(); 
  }
  else if (stepper == "y"){
    pos = stepperY.currentPosition();
  }
  else{
    Serial.println("Stepper "+ stepper + " not recognized in getGantryPosition!");
  }

  // return
  return pos;
}

// helper function to take 
void sendMeasurementSerial(int measurement, int gain, long x_pos, long y_pos){

  String msg = "$" + String(measurement) + ',' + String(gain) + ',' + String(x_pos) + ',' + String(y_pos) + '$';
  Serial.println(msg);
}

// helper function to query the current state of gain
int getPhotoluminometerGain(){

  return 1;
}

// function for a 2D raster i.e. a full plate scan
void raster2DScan(int stepsX, int stepsY, int stepSize){
  
  // raster in y taking steps in x
  for(int i=0; i < stepsX/2; i++){

    // raster in Y direction
    rasterScan("y", stepsY, stepSize);

    // take x step
    moveGantry("x", stepSize);

    // raster in -Y direction
    rasterScan("y", stepsY, -stepSize);

  }  

}

void setGantryAcceleration(int acceleration){

  stepperX.setAcceleration(acceleration);
  stepperY.setAcceleration(acceleration);
  
}

void idleGantry(){
  stepperX.disableOutputs();
  stepperY.disableOutputs();
}

void activateGantry(){
  stepperX.enableOutputs();
  stepperY.enableOutputs();
}
