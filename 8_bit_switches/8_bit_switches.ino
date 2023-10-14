#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 5

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define uint8_t byte

#define DATA_PIN 0
#define BUTTON_PIN 1  // the number of the pushbutton pin

// Define the array of leds
CRGB leds[NUM_LEDS];

// Global Variables:
byte colour = 0;//r,g,b,NO
byte redChannel   = 0b10000000 | 0b01000000;
byte blueChannel  = 0b10000000 & 0b01000000;
byte greenChannel = 0b10000000;
int buttonState = 0;  // variable for reading the pushbutton status


void setup() {
    //FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
    pinMode(BUTTON_PIN, INPUT_PULLUP);    
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonClick, RISING);

}

void loop() { 
  leds[0] = selectedColour();
  leds[1] = CRGB(redChannel,0,0);
  leds[2] = CRGB(0,greenChannel,0);
  leds[3] = CRGB(0,0,blueChannel);
  leds[4] = CRGB(redChannel,greenChannel,blueChannel);
  FastLED.show();
}
int selectedColour(){
  switch(colour) 
  {
    case 0:
      return CRGB::Red;
    case 1:
      return CRGB::Green;
    case 2:
      return CRGB::Blue;
    case 3:
      return CRGB::Black;
  }
}
void buttonClick(){
  if(colour == 3){
    colour=0;
  } else {
    colour += 1;
  }
  delay(10);
}
