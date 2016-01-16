#include <Adafruit_NeoPixel.h>
#include <Wire.h>

const int adressablePin1 = 12;
const int adressablePin2 = 13;
const int redPin = 11;
const int bluePin = 9;
const int greenPin = 10;
const int gndPin = 8;
const int length = 60;
int data[3];
int j = 0;
int disabledTimeout = 0;
boolean flag;
boolean allianceColor = false;//false is Red, true is Blue

Adafruit_NeoPixel strip = Adafruit_NeoPixel(length, adressablePin1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(length, adressablePin2, NEO_GRB + NEO_KHZ800);

void setup() {
  flag = true;
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(gndPin, OUTPUT);
  digitalWrite(gndPin, LOW);
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  strip.begin();
  strip2.begin();
  strip.show();
  strip2.show();
  randomSeed(analogRead(A0));
}

void loop() {
  if (data[2] == 0) {
    allianceColor = false;
  }
  else {
    allianceColor = true;
  }
  switch (data[0]) {
    case 0:
      setAll(0, 0, 0);
      if (allianceColor == true) {
        setGroundLights(0, 0, 255);
      }
      else {
        setGroundLights(255, 0, 0);
      }
      flag = true;
      break;
    case 1:
      if (allianceColor == true) {
        setGroundLights(0, 0, 255);
      }
      else {
        setGroundLights(255, 0, 0);
      }
      if (flag) {
        setAll(0, 0, 255);
      }
      for (int i = 0; i <= length; i++) {
        strip.setPixelColor(i, 255, 150, 0);
        strip2.setPixelColor(i, 255, 150, 0);
        if (i >= 1) {
          strip.setPixelColor(i - 11, 0, 0, 255);
          strip2.setPixelColor(i - 11, 0, 0, 255);
        }
        if (data[0] != 1) {
          break;
        }
        strip.show();
        strip2.show();
        delay(15);
      }
      flag = false;
      for (int i = 0; i <= length; i++) {
        strip.setPixelColor((length - 11) - i, 255, 150, 0);
        strip2.setPixelColor((length - 11) - i, 255, 150, 0);
        if (i <= 58) {
          strip.setPixelColor(length - i, 0, 0, 255);
          strip2.setPixelColor(length - i, 0, 0, 255);
        }
        if (data[0] != 1) {
          break;
        }
        strip.show();
        strip2.show();
        delay(15);
      }
      break;
    case 2:
      //Crazy Sequence
      for (int i = 0; i <= length; i++) {
        strip.setPixelColor(i, randColor());
        strip2.setPixelColor(i, randColor());
      }
      setGroundLights(random(0, 2) * 255, random(0, 2) * 255, random(0, 2) * 255);
      strip.show();
      strip2.show();
      break;
    case 3:
      //Blue and Gold
      if (allianceColor == true) {
        setGroundLights(0, 0, 255);
      }
      else {
        setGroundLights(255, 0, 0);
      }
      for (int i = 0; i <= length; i = i + 2) {
        strip.setPixelColor(i, 0, 0, 255);
        strip2.setPixelColor(i, 0, 0, 255);
      }
      for (int i = 1; i <= length; i = i + 2) {
        strip.setPixelColor(i, 255, 150, 0);
        strip2.setPixelColor(i, 255, 150, 0);
      }
      strip.show();
      strip2.show();
      break;
    case 4:
      //Disabled Sequence
      for (int i = 0; i <= 255; i++) {
        setAll(255 - i, 0, 0);
        setGroundLights(255 - i, 0, 0);
        if (data[0] != 4) {
          break;
        }
        delay(5);
      }
      for (int i = 0; i <= 255; i++) {
        setAll(i, 0, 0);
        setGroundLights(i, 0, 0);
        if (data[0] != 4) {
          break;
        }
      }
      delay(5);
      flag = true;
      break;
    case 5://Elevator Stuff
      if (allianceColor == true) {
        setGroundLights(0, 0, 255);
      }
      else {
        setGroundLights(255, 0, 0);
      }
      for (int i = 0; i <= data[1]; i++) {
        strip.setPixelColor(i, 255, 150, 0);
        strip2.setPixelColor(i, 255, 150, 0);
      }
      for (int i = length; i >= data[1]; i--) {
        strip.setPixelColor(i, 0, 0, 255);
        strip2.setPixelColor(i, 0, 0, 255);
      }
      strip.show();
      strip2.show();
      break;
    case 6:
      boolean once = true;
      if (once) {
        setAll(255, 150, 0);
        once = false;
      }
      for (int i = 0; i <= length; i++) {
        strip.setPixelColor(i, 0, 0, 255);
        strip2.setPixelColor(i, 0, 0, 255);
        strip.setPixelColor(60 - i, 0, 0, 255);
        strip2.setPixelColor(60 - i, 0, 0, 255);
        if (i >= 9) {
          strip.setPixelColor(i - 9, 255, 150, 0);
          strip2.setPixelColor(i - 9, 255, 150, 0);
        }
        if (data[0] != 5) {
          break;
        }
      }
      break;
  }
}
void setAll(int r, int g, int b) {
  for (int i = 0; i <= length; i++) {
    strip.setPixelColor(i, r, g, b);
    strip2.setPixelColor(i, r, g, b);
  }
  strip.show();
  strip2.show();
}
void setGroundLights(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(bluePin, b);
  analogWrite(greenPin, g);
}
uint32_t randBlueGold() {
  if (random(0, 2) == 0) {
    return strip.Color(0, 0, 255);
  }
  else {
    return strip.Color(255, 150, 0);
  }
}
uint32_t randColor() {
  return strip.Color(random(0, 2) * 255, random(0, 2) * 255, random(0, 2) * 255);
}
void receiveEvent(int howMany)
{
  j = 0;
  while (Wire.available() >= 1)
  {
    data[j] = Wire.read();
    j++;
  }
}
