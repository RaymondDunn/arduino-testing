#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_PIX 24
#define MAX_BRIGHTNESS 200

// initialize object
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIX, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {
  // set local vars
  uint32_t SIDE1_START = 0;
  uint32_t SIDE1_END = 23;

  // params for coloring
  uint32_t BLUE = strip.Color(0, 0, MAX_BRIGHTNESS, 0);
  uint32_t OFF = strip.Color(0, 0, 0, 0);

  // master params
  uint32_t RUN = 0;
  uint32_t LIGHT_ON_SECONDS = 2;
  uint32_t LIGHT_ON_MS = LIGHT_ON_SECONDS * 1000;
  uint32_t LIGHT_OFF_SECONDS = 4;
  uint32_t LIGHT_OFF_MS = LIGHT_OFF_SECONDS * 1000;

  // master loop for easily turning off
  if(RUN){

    // do side 1
    activate_array(SIDE1_START, SIDE1_END, BLUE);
    delay(LIGHT_ON_MS);

    // blank between
    activate_array(SIDE1_START, SIDE1_END, OFF);
    delay(LIGHT_OFF_MS);

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

