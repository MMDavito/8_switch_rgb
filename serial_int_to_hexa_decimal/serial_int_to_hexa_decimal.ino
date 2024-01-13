//Many thanks to Nick Gammon for the basis of this code
//http://www.gammon.com.au/serial

//https://github.com/alikabeel/Letters-and-Numbers-Seven-Segment-Display-Library
#include <SevenSegmentDisplay.h>
volatile byte b_in = 0b10000000;
volatile byte counter = 0b00000000;
const byte reset = 0xFF;



//MostSignificant, to LSB
//First 3 is decimal, last two is Hex
byte digitPins[] = {11, 12, 13, 9, 10};

SevenSegmentDisplay screenName(2, 8, 7, 6, 5, 3, 4, NULL, false);//CommonCathode no decimal-point


const unsigned int MAX_MESSAGE_LENGTH = 12;
char decimalArray [3] = {'0', '0', '0'};
String decimal = "";
String hex = "";
char hexArray[2] = {'0', '0'};

void byteToArrays(byte value) {
  decimal = "";
  hex = "";


  if (value < 0x10) hex += "0";
  if (value < 100) decimal += "0";
  if (value < 10) decimal += "0";

  hex += String(value, HEX);
  hex.toUpperCase();

  decimal += String(value, DEC);

  decimal.toCharArray(decimalArray, 4);
  if (value >= 100 && value < 200) decimalArray[0] = '1';
  else if (value >= 200) decimalArray[0] = '2';
  else decimalArray[0] = '0';

  hex.toCharArray(hexArray, 3);
}


void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 5; i++)
  {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], LOW);
  }

}

byte readSerial() {
  delay(5);
  String recieved = "";
  int numChars = 0;
  while (Serial.available()) {
    byte b = Serial.read() - '0';
    if (b == 218) break;
    recieved += b;
    numChars ++;
    //if(numChars == 3) return recieved.toInt();
  }
  return recieved.toInt();
}

void loop() {
  //Check to see if anything is available in the serial receive buffer
  int numChars = 0;
  if (Serial.available())
  {

    byte inByte = readSerial();
    byteToArrays(inByte);
  }
  for (int i = 0; i < 5; i++)
  {
    if (i > 2)
    {
      screenName.displayCharacter(hexArray[i - 3]);
      digitalWrite(digitPins[i], HIGH);
      long start = micros();
      while ((micros() - start) < 500) //do nothing
        if ((micros() - start) > 10000) break;
    }
    else
    {
      screenName.displayCharacter(decimalArray[i]);
      digitalWrite(digitPins[i], HIGH);
      long start = micros();
      while ((micros() - start) < 100) //do nothing
        if ((micros() - start) > 10000) break;
    }
    digitalWrite(digitPins[i], LOW);
  }
}
