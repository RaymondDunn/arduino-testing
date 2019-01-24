#include <Adafruit_NeoPixel.h>

// helpful params
uint32_t RUN = 3;
uint32_t DELAY_SECONDS = 600;
uint32_t BLANK_SECONDS = 1; // deprecated

// set local vars
uint32_t SIDE1_START = 1;
uint32_t SIDE1_END = 1;
uint32_t SIDE2_START = 9;
uint32_t SIDE2_END = 9;

#define PIN 6
#define NUM_PIX 16
#define MAX_BRIGHTNESS 255

// initialize object
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIX, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {


  // params for coloring
  uint32_t WHITE = strip.Color(0, 0, 0, MAX_BRIGHTNESS);
  uint32_t RED = strip.Color(MAX_BRIGHTNESS, 0, 0, 0);
  uint32_t BLUE = strip.Color(0, 0, MAX_BRIGHTNESS, 0);
  uint32_t GREEN = strip.Color(0, MAX_BRIGHTNESS, 0, 0);
  uint32_t OFF = strip.Color(0,0,0,0);

  // timing params
  uint32_t DELAY_MS = DELAY_SECONDS * 1000;
  uint32_t BLANK_MS = BLANK_SECONDS * 1000;

  // master loop for easily turning off
  if(RUN){

    // start with blank
    delay(DELAY_MS);
    
    // do side 1
    activate_array(SIDE1_START, SIDE1_END, WHITE);
    delay(DELAY_MS);

    // blank between
    activate_array(SIDE1_START, SIDE2_END, OFF);
    delay(DELAY_MS);

    // do side 2
    activate_array(SIDE2_START, SIDE2_END, WHITE);
    delay(DELAY_MS);

    // blank between
    activate_array(SIDE1_START, SIDE2_END, OFF);
    delay(DELAY_MS);

    // stop iteratin3
    RUN = RUN - 1;
  }
  
}

static void activate_array(uint32_t px_start, uint32_t px_end, uint32_t color){

  // iterate through pixels, if in range then turn on, otherwise turn off
  for(int i = 0; i < NUM_PIX; i++){

    // if side 1, display pixel. otherwise, wash
    if ((i >= px_start) && (i <= px_end)){
      strip.setPixelColor(i, color);
    }
    else{
      strip.setPixelColor(i,0);
    }
  }

  // push buffer
  strip.show();
}

