void ICACHE_RAM_ATTR buttonClick();

#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 5

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define uint8_t byte

#define DATA_PIN D4
#define BUTTON_PIN D2  // the number of the pushbutton pin
#define WR_EN D3  // WriteEngage, active high
//Will expand number of bits once I start soldering more ATMEGA328
byte switches [] = { 0, 0,D3,D4,D5,D6,D7,D8}; //LSB => MSB

// Define the array of leds
CRGB leds[NUM_LEDS];

// Global Variables:
byte colour = 0;//r,g,b,NO
byte redChannel   = 0b00000000 | 0b01000000;
byte blueChannel  = 0b10000001 & 0b01000001;
byte greenChannel = 0b01000000;

unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

unsigned long led_write_time = 0;  
unsigned long last_led_write_time = 0; 

int buttonState = 0;  // variable for reading the pushbutton status

void buttonClick() {
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    last_button_time = button_time;
    if (colour == 3) {
      colour = 0;
    } else {
      colour += 1;
    }
  }
}


void setup() {
  //FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  for (unsigned i = 7; i < 8; i++)  {
        pinMode (switches [i], INPUT);
  }
  pinMode(WR_EN, INPUT);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonClick, RISING);
}

void loop() {
  if(digitalRead(WR_EN) == HIGH) {
  //if(true) {
    setChannelFromSwitches();
  }
  leds[0] = selectedColour();
  leds[1] = CRGB(redChannel, 0, 0);
  leds[2] = CRGB(0, greenChannel, 0);
  leds[3] = CRGB(0, 0, blueChannel);
  leds[4] = CRGB(redChannel, greenChannel, blueChannel);
  led_write_time = millis();
  if (led_write_time - last_led_write_time > 250)
  {
    FastLED.show();
    last_led_write_time = led_write_time;    
  }
}
int selectedColour() {
  switch (colour)
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

void setChannelFromSwitches() {
  //Call this only if WR_EN is enabled
  byte temp = 0b00000000;
  //TODO: reset to number of used switches later!
  for (unsigned i = 7; i < 8; i++) {
    bool isHigh = digitalRead(switches[i]) == HIGH;
    if (isHigh) {
      switch (i) {
        case 0:
          temp = temp | 0b00000001;
          break;
        case 1:
          temp = temp | 0b00000010;
          break;
        case 2:
          temp = temp | 0b00000100;
          break;
        case 3:
          temp = temp | 0b00001000;
          break;
        case 4:
          temp = temp | 0b00010000;
          break;
        case 5:
          temp = temp | 0b00100000;
          break;
        case 6:
          temp = temp | 0b01000000;
          break;
        case 7:
          temp = temp | 0b10000000;
          break;
      }
      delay(10);
    }
  }
  switch (colour)
  {
    case 0:
      redChannel=temp;
      return;
    case 1:
      greenChannel=temp;
      return;
    case 2:
      blueChannel=temp;
      return;
    case 3:
      return;
  }
}
