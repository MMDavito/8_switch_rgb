//void ICACHE_RAM_ATTR buttonClick();
#include <EEPROM.h>
#include <FastLED.h>


// How many leds in your strip?
#define NUM_LEDS 5

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define uint8_t byte

#define BUTTON_PIN 2  // the number of the pushbutton pin
//Pin 3 could be used as interrupt
#define DATA_PIN 4 // Pin to connect resistor and RGB-strip to
#define WR_EN A4 // WriteEngage, active high
#define MEM_EN A5 // Will allow to write to serial; either memory or switch state (TOOD implement!)

//Will expand number of bits once I start soldering more ATMEGA328
byte switches [] = { 5, 6, 7, 8, 9, 10 , 12, 13}; //LSB => MSB
byte switchValues = 0b10000000; 
byte lastWrittenToSerial = 0b00000000;

// Define the array of leds
CRGB leds[NUM_LEDS];

// Global Variables:
volatile byte color = 0;//r,g,b,NO color
byte colorAddresses[] = {1, 2, 3, 4};//Addresses for EEPROM/rgb; NONE
byte channelValues[] = {0b10000000, 0b10000000, 0b10000000, 0b00000000};//RGB; NONE

unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

unsigned long led_write_time = 0;  
volatile unsigned long last_led_write_time = 0; 

int buttonState = 0;  // variable for reading the pushbutton status

void buttonClick() {
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    last_button_time = button_time;
    if (color == 3) {
      color = 0;
    } else {
      color += 1;
    }
    last_led_write_time = last_led_write_time - 500;
  }
}

long selectedColor() {
  switch (color)
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

void readSwitches() {
  //Call this only if WR_EN is enabled
  byte temp = 0b00000000;
  for (unsigned i = 0; i < 8; i++) {
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
  switchValues = temp;
}

void setLedColors(){
  leds[0] = selectedColor();
  leds[1] = CRGB(channelValues[0], 0, 0);
  leds[2] = CRGB(0, channelValues[1], 0);
  leds[3] = CRGB(0, 0, channelValues[2]);
  
  leds[4] = CRGB(channelValues[0], channelValues[1], channelValues[2]);
}
void writeToSerial(){
  if(digitalRead(MEM_EN) == HIGH){
    if(lastWrittenToSerial == channelValues[color]) return;
    //Write from memory:
    Serial.println(channelValues[color]);
    lastWrittenToSerial = channelValues[color];
  }else{
    //Write from switches:
    if(lastWrittenToSerial == switchValues) return;
    Serial.println(switchValues);
    lastWrittenToSerial = switchValues;
  }
}

void setup() {
  Serial.begin(9600);
  
  //Initilize EEPROM if not initilized, else read from EEPROM
  byte initilized = EEPROM.read(0);
  if(initilized > 0){
    EEPROM.write(0,0);
    for(int i=0; i<4; i++){
      EEPROM.write(colorAddresses[i],channelValues[i]);
    }
  }
  else{
    for(int i=0; i<4; i++){
      channelValues[i] = EEPROM.read(colorAddresses[i]);
    }
  }
  
  //FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  for (unsigned i = 0; i < 8; i++)  {
        pinMode (switches [i], INPUT);
  }
  pinMode(WR_EN, INPUT);
  pinMode(MEM_EN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonClick, RISING);
  
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  
  setLedColors();
  led_write_time = millis();
  FastLED.show();
  writeToSerial();
}

void loop() {
  if(digitalRead(WR_EN) == HIGH) {
  //if(false) {
    readSwitches();
    channelValues[color] = switchValues;
    EEPROM.put(colorAddresses[color], switchValues);
  }
  led_write_time = millis();
  if (led_write_time - last_led_write_time > 250)
  {
    setLedColors();
    FastLED.show();
    last_led_write_time = millis();
    writeToSerial();
  }
}
