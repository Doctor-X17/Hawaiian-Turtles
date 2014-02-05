#include <EEPROM.h>
#include <LiquidTWI.h>
#include <Wire.h>
#include <SdFat.h>

LiquidTWI lcd(0);
SdFat sd;
SdFile myFile;


int butPins[] = { but1, but2, but3, but4 };
int butPinCount = 4;

int ledPins[] = { aer, slu, cwp };
int ledPinCount = 3

// Pin Setup
const int aer = 14;
const int slu = 13;
const int cwp = 12;
const int sw1 = A3;
const int sw2 = 2;
const int amp = A0;
const int volt = 3;
const int siren = A1;
const int sirena = 23;
const int light = A2;
const int but1 = 0;
const int but2 = 20;
const int but3 = 18;
const int but4 = 19;
const int chipSelect = 4;

void setup() {
 lcd.begin(16,2);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.print("Turtle Power!");
  pinMode(aer, OUTPUT);
  pinMode(slu, OUTPUT);
  pinMode(cwp, OUTPUT);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(but1, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);
  pinMode(but3, INPUT_PULLUP);
  pinMode(but4, INPUT_PULLUP);  
  delay(500);
  lcd.clear();
}

void loop() {
  
}
