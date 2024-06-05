#include <SPI.h>
#include <EEPROM.h>
#include <FastLED.h>
#define NUM_LEDS 5

#define BUTTON 3 //5
#define SW_MEM 6 //12
/*
  //Correct order:

  #define SW_WR A4 //27
  #define SW_SERR A5 //28
*/
//More ergonomic order (mostly relevant when using it as a calculator):
#define SW_SERR A4 //27
#define SW_WR A5 //28
/**/
#define OUTPUT_ENABLE 2 //4
#define RCK_INPUT 7 //13
#define RCK_OUTPUT 8 //14
#define RCK_TRANS 9 //15
#define RGB1_PIN 4//6

const bool isDebug = false;
// If LED-strip is letting Current and Signal pass Right to Left instead of Left to Right:
const bool isLedsReversed = true;

//if SW_WR is HIGH when SW_SERR has been HIGH, Non-Volatile: EEPROM 5.
volatile bool isOff = false;
const byte eepromOff = 5; //EEPROM address of above boolean.

volatile bool isCounter = false;
volatile bool isBlackout = false;

volatile unsigned long lastCount = 0;
volatile unsigned long updateInterval = 100;

// Define the array of LEDs
CRGB leds[NUM_LEDS];

volatile byte color = 0; //r,g,b,NO color
const byte colorAddresses[] = {1, 2, 3, 4}; //Addresses for EEPROM/R,G,B; NONE
volatile byte channelValues[] = {0b10000000, 0b10000000, 0b10000000, 0b00000000}; //R,G,B; NONE

const byte customChars[] = {
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11100110, // 9
  B11101110, // a
  B00111110, // b
  B10011100, // c
  B01111010, // d
  B10011110, // e
  B10001110  // f
};
/*
  //Logic order output addresses:
  const byte outputs[6]  =
  {
  0b10000000,
  0b01000000,
  0b00100000,
  0b00010000,
  0b00001000,
  0b00000100,
  };
*/
//Actual order output addresses:
const byte outputs[6]  =
{
  0b00100000,
  0b01000000,
  0b10000000,
  0b00010000,
  0b00001000,
  0b00000100,
};
/**/

volatile byte outputValues[6] = {
  0b11000000,
  0b01000000,
  0b00100000,
  0b00010000,
  0b00001000,
  0b00000101,
};

volatile char outputChars[6];
volatile byte counter = 0x00;

volatile byte switchValues = 0b10000000;

volatile unsigned long button_time = 0;
volatile unsigned long last_button_time = 0;

volatile unsigned long buttonTime = 0;
volatile unsigned long lastButtonActive = 0;
volatile bool buttonToggled = false;

void buttonHigh() {
  if (buttonTime == 0) {
    buttonTime = millis();
  }
  else if (buttonTime - lastButtonActive > 100 && buttonToggled == false)
  {
    if (isCounter) {
      switch (updateInterval)
      {

        case 5000:
          updateInterval = 2000;
          break;
        case 2000:
          updateInterval = 1000;
          break;
        case 1000:
          updateInterval = 500;
          break;
        case 500:
          updateInterval = 250;
          break;
        case 250:
          updateInterval = 100;
          break;
        case 100:
          updateInterval = 50;
          break;
        case 50:
          updateInterval = 10;
          break;
        case 10:
          updateInterval = 5000;
          break;
        default:
          updateInterval = 100;
          break;
      }
    }
    else {
      if (color == 3) {
        color = 0;
      } else {
        color += 1;
      }
      setLedColors();
    }
    buttonToggled = true;
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

/**
   Sets and shows the selected color according to different modes.
*/
void setLedColors() {
  if (isOff) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
  } else if (isBlackout) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(channelValues[0], channelValues[1], channelValues[2]);
    }
  }
  else {
    if (isLedsReversed) {
      leds[4] = selectedColor();
      leds[3] = CRGB(channelValues[0], 0, 0);
      leds[2] = CRGB(0, channelValues[1], 0);
      leds[1] = CRGB(0, 0, channelValues[2]);

      leds[0] = CRGB(channelValues[0], channelValues[1], channelValues[2]);
    }
    else {
      leds[0] = selectedColor();
      leds[1] = CRGB(channelValues[0], 0, 0);
      leds[2] = CRGB(0, channelValues[1], 0);
      leds[3] = CRGB(0, 0, channelValues[2]);

      leds[4] = CRGB(channelValues[0], channelValues[1], channelValues[2]);
    }
  }
  FastLED.show();
}

void printByte(byte val) {
  for (int i = 7; i >= 0; i--)
  {
    bool b = bitRead(val, i);
    Serial.print(b);
  }
}

byte charToByte(char ch) {
  // Validate input character
  if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F')) {
    // Handle decimal or hexadecimal character
    int value;
    if (ch >= '0' && ch <= '9') {
      value = ch - '0';
    } else {
      value = ch - 'A' + 10; // Convert hex char to numerical value (A-F: 10-15)
    }
    return value;
    //return value << 4; // Shift left by 4 bits (No idea why I would want to?)
  } else {
    // Return 0 for invalid characters
    return 0;
  }
}
/*
  Given a value this will change the different arrays so their values are representative for later use.
*/
void setOutputValues(byte value) {
  // Convert to decimal string manually
  String strValue = String(value);
  // Handle single-digit values
  if (strValue.length() == 1)
    strValue = "00" + strValue; // Prepend leading zeros
  else if (strValue.length() == 2)
    strValue = "0" + strValue; // Prepend leading zeros
  strValue.toCharArray(outputChars, sizeof(outputChars)); // Copy to char array
  for (int i = 0; i < 3; i++) {
    outputValues[i] = customChars[charToByte(outputChars[i])];
  }

  // Convert to hexadecimal string manually
  byte nibble;
  for (int i = 0; i < 2; i++) {
    nibble = (value >> (4 * i)) & 0x0F;
    outputChars[4 - i] = (nibble < 10) ? '0' + nibble : 'A' + nibble - 10;
    outputValues[4 - i] = customChars[nibble];
  }
  outputChars[5] = char(value);
  outputValues[5] = value;
}

/*
  Sets the decimal displays to display "OFF", all other displays are black/empty.
*/
void setOutputValuesOff() {
  // Convert to decimal string manually
  String strValue = "0FF";
  strValue.toCharArray(outputChars, sizeof(outputChars)); // Copy to char array
  for (int i = 0; i < 3; i++) {
    outputValues[i] = customChars[charToByte(outputChars[i])];
  }

  for (int i = 0; i < 2; i++) {
    outputChars[4 - i] = 0;
    outputValues[4 - i] = 0x00;
  }
  outputChars[5] = 0;
  outputValues[5] = 0x00;
}

/**
  Reads inputs from 74hc165
*/
byte readInputs() {
  digitalWrite(RCK_INPUT, LOW);
  digitalWrite(RCK_INPUT, HIGH);
  //asm("nop\n nop\n");              //some delay

  byte switches = SPI.transfer(0); //get the switches states
  switchValues = switches;
  return switches;
}

void setup() {
  //Serial.begin(115200);//WORKS
  //Serial.begin(1000000);//Works
  Serial.begin(2000000);//Works

  //Initilize EEPROM if not initialized, else read from EEPROM
  byte initialized = EEPROM.read(0);
  //byte initialized = 0xFF;
  if (initialized > 0) {
    EEPROM.write(0, 0);
    for (int i = 0; i < 4; i++) {
      EEPROM.write(colorAddresses[i], channelValues[i]);
    }
    EEPROM.write(eepromOff, 0);
  }
  else {
    for (int i = 0; i < 4; i++) {
      channelValues[i] = EEPROM.read(colorAddresses[i]);
    }
    if (EEPROM.read(eepromOff) == 0x55) isOff = true;
    else isOff = false;
  }

  SPI.begin();

  //SPI.setDataMode(SPI_MODE0); // default - don't need
  SPI.setBitOrder(LSBFIRST); // MSBFIRST is default - comment out if determined as needed.

  pinMode(BUTTON, INPUT);
  pinMode(SW_MEM, INPUT);
  pinMode(SW_WR, INPUT);
  pinMode(SW_SERR, INPUT);

  pinMode(OUTPUT_ENABLE, OUTPUT);
  pinMode(RCK_INPUT, OUTPUT);
  pinMode(RCK_OUTPUT, OUTPUT);
  pinMode(RCK_TRANS, OUTPUT);

  digitalWrite(OUTPUT_ENABLE, HIGH);
  digitalWrite(RCK_INPUT, HIGH);
  digitalWrite(RCK_OUTPUT, HIGH);
  digitalWrite(RCK_TRANS, HIGH);

  FastLED.addLeds<WS2812B, RGB1_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical

  setLedColors();
  delay(100);
}

void regularMode() {
  if (digitalRead(BUTTON) == HIGH) {
    buttonHigh();
  }
  else if (buttonTime != 0) {
    lastButtonActive = buttonTime;
    buttonTime = 0;
    buttonToggled = false;
  }

  byte inputs = readInputs();

  if (digitalRead(SW_WR) == HIGH && inputs != channelValues[color]) {
    channelValues[color] = inputs;
    // Could use a array and only write if the value actually changed.
    // Memory is only rated for 100k cycles.
    // Note: This function uses EEPROM.update() to perform the write, so does not rewrites the value if it didn't change.
    EEPROM.put(colorAddresses[color], inputs);
    setLedColors();
  }

  if (digitalRead(SW_MEM) == LOW)
  {
    setOutputValues(inputs);
  }
  else {
    setOutputValues(channelValues[color]);
  }

  if (isDebug) {
    for (int i = 0; i < 6; i++)
    {
      Serial.print("Output: ");
      printByte(outputs[i]);
      Serial.println("");
      Serial.print("Char: ");
      Serial.print(i);
      Serial.print(", is: ");
      Serial.println(outputChars[i]);
    }
    Serial.println("___________________");
  }
}

void offMode() {
  if (digitalRead(SW_WR) != HIGH) {
    isBlackout = false;
    isOff = false;
    EEPROM.put(eepromOff, 0);
    setLedColors();
    return;
  }
  if (outputChars[2] != 'F') {
    EEPROM.put(eepromOff, 0x55);
    setLedColors();
    setOutputValuesOff();
  }
}
void blackoutMode() {
  if (digitalRead(SW_SERR) != HIGH) {
    isBlackout = false;
    isCounter = false;
    setLedColors();
    return;
  }
  if (outputValues[0] != 0) {
    for (int i = 0; i < 6; i++) {
      outputValues[i] = 0x00;
      outputChars[i] = 0;
      setLedColors();
    }
  }
  delay(100);
}
void counterMode() {
  if (digitalRead(SW_MEM) != HIGH || digitalRead(SW_SERR) != HIGH) {
    //updateInterval = 100;
    isBlackout = false;
    isCounter = false;
    setLedColors();
    return;
  }
  if (digitalRead(BUTTON) == HIGH) {
    buttonHigh();
  }
  else if (buttonTime != 0) {
    lastButtonActive = buttonTime;
    buttonTime = 0;
    buttonToggled = false;
  }
  if (millis() - lastCount >= updateInterval) {
    if (counter == 255)
      counter = 0;
    else
      counter ++;
    setOutputValues(counter);
    lastCount = millis();
  }
}

void loop() {
  if (isOff) {
    offMode();
  } else if (isCounter) {
    counterMode();
  }
  else if (isBlackout) {
    blackoutMode();
  }
  else {
    regularMode();
  }
  long start = millis();
  while (millis() - start < 10) {
    //while (millis() - start < 50) {
    for (int i = 0; i < 6; i ++) {
      byte temp = outputValues[i];

      digitalWrite(OUTPUT_ENABLE, HIGH);
      digitalWrite(RCK_OUTPUT, LOW);
      digitalWrite(RCK_TRANS, LOW);

      SPI.transfer(outputs[i]);
      SPI.transfer(temp);

      digitalWrite(RCK_OUTPUT, HIGH);
      digitalWrite(RCK_TRANS, HIGH);
      digitalWrite(OUTPUT_ENABLE, LOW);

      delayMicroseconds(100);
      //delay(100)
    }
  }
  digitalWrite(OUTPUT_ENABLE, HIGH);
  if (digitalRead(SW_SERR)) {
    if (isBlackout == false) {
      //Turn on the LEDS in accordance with this mode.
      isBlackout = true;
      blackoutMode();
      //Wait until next cycle to check the other ones? (Decided not to for some reason)
      //return;
    }
    if (digitalRead(SW_MEM)) isCounter = true;
    if (digitalRead(SW_WR)) isOff = true;
  }
}
