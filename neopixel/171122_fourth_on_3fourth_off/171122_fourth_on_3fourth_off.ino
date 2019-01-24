#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_PIX 16
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
  uint32_t SIDE1_END = 3;
  uint32_t SIDE2_START = 4;
  uint32_t SIDE2_END = 7;
  uint32_t SIDE3_START = 8;
  uint32_t SIDE3_END = 11;
  uint32_t SIDE4_START = 12;
  uint32_t SIDE4_END = 15;

  // params for coloring
  uint32_t WHITE = strip.Color(0, 0, 0, MAX_BRIGHTNESS);
  uint32_t RED = strip.Color(MAX_BRIGHTNESS, 0, 0, 0);
  uint32_t BLUE = strip.Color(0, 0, MAX_BRIGHTNESS, 0);

  // master params
  uint32_t RUN = 1;
  uint32_t COLOR_TO_DISPLAY = RED;
  uint32_t DELAY_SECONDS = 60;
  uint32_t DELAY_MS = DELAY_SECONDS * 1000;

  // master loop for easily turning off
  if(RUN){
    activate_array(SIDE1_START, SIDE1_END, COLOR_TO_DISPLAY);
    delay(DELAY_MS);
    activate_array(SIDE2_START, SIDE2_END, COLOR_TO_DISPLAY);
    delay(DELAY_MS);
    activate_array(SIDE3_START, SIDE3_END, COLOR_TO_DISPLAY);
    delay(DELAY_MS);
    activate_array(SIDE4_START, SIDE4_END, COLOR_TO_DISPLAY);
    delay(DELAY_MS);
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
