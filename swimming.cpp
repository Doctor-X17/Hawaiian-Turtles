//Turtles are cool.


#include <EEPROM.h>
#include <LiquidTWI.h>
#include <Wire.h>
#include <SdFat.h>

LiquidTWI lcd(0);
SdFat sd;
SdFile myFile;

// Debug
int aerTime = 3; // in seconds, should be 15 min
const int aerLiteTime = 1; in seconds, should be 60
int filTime = 3000; // milliseconds that the fill pump is on for pulse
int filTimeSec = filTime / 1000;
const int filPause = 180; // number of seconds between fill pulse and end
const int sludgeReturnTime = 6; // in seconds, should be calculated on volume
const int denitTime = 3000; // in millis -- runtime of aerator during denit pulse
const int denitFillTime = 5000; // in millis
const int cyclePause = 3 // in seconds, should be about 15 min
const int settlingTime = 10 // in seconds, should be 105 min
const int hrCyclePause = 3360; // 56 min pause
const int debounce = 300;
const int timeout = 25; // equals 1/5 of a second, should be 100
const int swTimeout = 25; // equals 1/5 of a second,

// Global Variables
int cwpSeconds;
int cycleSteps;

int butPins[] = { but1, but2, but3, but4 };
int butPinCount = 4;

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

// EEPROM Addresses
const int versionAddr = 0;
const int delayedStartAddr = 1;
const int cycleStepsAddr = 30; // 30 Block is setup
const int aerAddr = ;
const int sluAddr = ;
const int cwpAddr =  ; // base address for cwp runtimes -- add others
const int cwpSecRecAddr = 
const int cwpMinRecAddr = //storage for runtimes
const int cwpHrRecAddr = 


// Defaults
const int fillTimeAwayDefault = 3;
const int fillTimeVacaDefault = 10;
const int sluReturnTimeDefault = 15;
const int aerTimeHomeDefault = 12;
const int aerTimeAwayDefault = 1;
const int cwpTimeDefault = 30;
const int cycleHoursDefault = 8;
const int denitPulseDelay = 3000;
const int cycleStepsDefault = 6;

// Strings
String switchStr = "1Aer 2Slu 3CWP"; // 14 POS
String cwpStr = "3CWP    "; // 8 POS
String filString = "Filling "; // 8 POS
String denitString = "Denit   "; // 8 POS
String pauseString = "Pause   "; // 8 POS
String aerString = "Aeration"; // 8 POS
String setString = "Settling"; // 8 POS
String decString = "Decant  "; // 8 POS
// could be local -- String nameString = "        "; // 8 POS

// Local variables used
// steps in normalCycle(); 

void setup() {

  lcd.begin(16,2);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.print("PekaSys Bubbler");
  pinMode(aer, OUTPUT);
  pinMode(slu, OUTPUT);
  pinMode(cwp, OUTPUT);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(but1, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);
  pinMode(but3, INPUT_PULLUP);
  pinMode(but4, INPUT_PULLUP);  
  
  cycleSteps = EEPROM.read(cycleStepsAddr);
}
  
void loop() {
	filling();  // needs to be updated with away/vacation
	boolean switchState = digitalRead(sw1);
	if (switchState == HIGH) {
		aerationLite();
		addSeconds(aerAddr, aerLiteTime);
		addSeconds(sluAddr, filTimeSec);
	}
	else {
		normalCycle();
	}
}


//********************************************
byte checkButtons() { // checks for button press and executes action
  if(digitalRead(but1) == LOW) {
    lcd.clear();
    delay(300);
    return but1;
  }
  else if(digitalRead(but2) == LOW) {
    lcd.clear();
    delay(300);
    return but2;
  }
  else if(digitalRead(but3) == LOW) {
    lcd.clear();
    delay(300);
    return but3;
  }
  else {
    return 100;
  }
}

//********************************************
void filling() {

  lcd.setCursor(0,0);
  lcd.print(filString);
  digitalWrite(slu, HIGH);
  delay(filTime); // 1000 --> 3000
  digitalWrite(slu, LOW);
  menuDelay(filPause); // 180
}
  
//********************************************
void aerationLite() {
	digitalWrite(aer, HIGH);
	menuDelay(aerLiteTime);
	digitalWrite(aer, LOW);
	menuDelay(hrCyclePause);
}

//********************************************
void normalCycle(int steps) {  // steps comes from cycleSteps
	for ( int s = 0; s < steps; s++ ) {
		if (s > 0) {
			filling();
		}
		normStep();
	}
	lcd.setCursor(0,0);
	lcd.print(setString);
	menuDelay(settlingTime);
	lcd.setCursor(0,0);
	lcd.print(decString);
	digitalWrite(cwp, HIGH);
	cwpSeconds = 0;
	while (digitalRead(sw1) == LOW) {
		cwpSeconds++;
		delay(1000);
	}
	unsigned int aerSeconds = cycleSteps * ( aerTime + (denitTime/333))
	addSeconds(aerAddr, aerSeconds);
	unsigned int sluSeconds = sludgeReturnTime + ( filTime * cycleSteps );
	addSeconds(sluAddr, sluSeconds);
	addSeconds(cwpAddr, cwpSeconds);
}

//********************************************
void normStep() {
	lcd.setCursor(0,0);
	lcd.print(aerString);
	digitalWrite(aer, HIGH);
	menuDelay(aerTime);
	digitalWrite(aer, LOW);
	lcd.setCursor(0,0);
	lcd.print(pauseString);
	menuDelay(cyclePause); 
	denit();
	menuDelay(cyclePause);
	denit();
	menuDelay(cyclePause);
	denit();
	delay(denitFillTime); 
}

//********************************************
void runDefault() { // set EEPROMs to default values
	
}

//********************************************
void menuDelay(int numSecs) { 
  unsigned long menuDelayCount = numSecs * 5;
  for (unsigned long xi=0; xi < menuDelayCount; xi++) {
    boolean menuButtonState = digitalRead(but1);
    if (menuButtonState == LOW) {
      menu(); 
    }
   delay(200);
  }
}

//********************************************
void denit() {
	lcd.setCursor(0,0);
	lcd.print(denitString);
	digitalWrite(aer, HIGH);
	delay(denitTime);
	digitalWrite(aer, LOW);
	lcd.setCursor(0,0);
	lcd.print(pauseString);
}

//********************************************
void AddSeconds(byte addr, int addSec) { // add addr and addSec to local
	int curSecs = EEPROM.read(addr);
	int curMins = EEPROM.read(addr + 1);
	int curHrs = EEPROM.read(addr + 2);
	int curMult = EEPROM.read(addr + 3);
	curSecs += addSec;
	int newSecs = curSecs % 60;
	curMins += addSec / 60;
	int newMins = curMins % 60;
	curHrs += curMins / 60;
	int newHrs = curHrs % 255;
	curMult += curHrs / 255;
	int newMult = curMult;
	
	EEPROM.write(addr, newSecs);
	EEPROM.write(addr + 1, newMins);
	EEPROM.write(addr + 2, newHrs);
	EEPROM.write(addr + 3, newMult);
}

//********************************************
// Test this part with the String passed to the function ***********
void manualSwitch(byte switchPin, String nameString) { // add nameString
	lcd.clear();
	lcd.print(nameString);
	lcd.print("     Off");
	lcd.setCursor(0,1);
	lcd.print("1. On / 2. Off");
	for( int swTime = 0; swTime < switchTimeout; swTime++) {
		byte butPressed = checkButtons();
		if (butPressed == but1) {
			digitalWrite(switchPin, HIGH);
			lcd.setCursor(13,0);
			lcd.Print("On ");
		}
		else if (butPressed == but2) {
			digitalWrite(switchPin, LOW);
			lcd.setCursor(13,0);
			lcd.print("Off");
		}
		else if (butPressed == but4) {
			break;
		}
		delay(200);
	}
}

//********************************************
//********************************************
//********************************************
void menu() {
	lcd.clear();
	lcd.print("1.Runtimes 2.Run");
	lcd.setCursor(0,1);
	lcd.print("3.Settings 4.End");
	delay(debounce);
	for(int m = 0; m < timeout; m++) {
		byte butPress = checkButtons();
		switch (butPress) {
			case 'but1':
				menu1();
				break;
			case 'but2':
				menu2();
				break;
			case 'but3':
				menu3();
				break;
			case 'but4':
				break;
			default:
				delay(200);
		}
	}
}

//********************************************
void menu1() {
	lcd.clear();
	lcd.print("1.Aer 2.Sludge");
	lcd.setCursor(0,1);
	lcd.print("3.CWP 4.Cancel");
	delay(debounce);
	for(int m = 0; m < timeout; m++) {
		byte butPress = checkButtons();
		switch (butPress) {
			case 'but1':
				menu11();
				break;
			case 'but2':
				menu12();
				break;
			case 'but3':
				menu13();
				break;
			case 'but4':
				break;
			default:
				delay(200);
		}
	}
}

//********************************************
void menu2() {
	lcd.clear();
	lcd.print("1.Aer 2.Sludge");
	lcd.setCursor(0,1);
	lcd.print("3.CWP 4.Cancel");
	delay(debounce);
	for(int m = 0; m < timeout; m++) {
		byte butPress = checkButtons();
		switch (butPress) {
			case 'but1':
				manualSwitch(aer, "Aerator ");
				break;
			case 'but2':
				manualSwitch(slu, "Sludge  ");
				break;
			case 'but3':
				manualSwitch(cwp, "CWP     ");
				break;
			case 'but4':
				break;
			default:
				delay(200);
		}
	}
}

//********************************************
void menu3() {
	lcd.clear();
	lcd.print("");
	lcd.setCursor(0,1);
	lcd.print("");
	delay(debounce);
	for(int m = 0; m < timeout; m++) {
		byte butPress = checkButtons();
		switch (butPress) {
			case 'but1':
				menu31();
				break;
			case 'but2':
				menu32();
				break;
			case 'but3':
				menu33();
				break;
			case 'but4':
				break;
			default:
				delay(200);
		}
	}
}

//********************************************
//********************************************

//********************************************

//********************************************





