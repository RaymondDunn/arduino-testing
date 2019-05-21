//www.elegoo.com
//2016.12.08

// import tone library and set to C6 (not necessary... but C6 = 1047)
int note = 1047;

// define pin
int pin = 8;
 
// duration of notes
int duration = 1000; 

 
void setup() {
  // don't do anything...
}


// control var
int RUN = 1;

void loop() {  

  if(RUN){
    // play tone
    tone(pin, note, duration);
     
    // delay
    delay(1000);
  }

   
}
