int startVerify;



  /*int startup = EEPROM.read(delayedStartAddr);
  if (startup == 1) {
    lcd.print("Ready for water");
    lcd.setCursor(0,1);
    lcd.print("Test pumps 1-3");
    while (digitalRead(sw1) == HIGH) {
      delay(5000);
    }
    lcd.clear();
    lcd.print("Water detected");
    lcd.setCursor(0,1);
    lcd.print("Starting");
    startVerify = 0;
    while (startVerify < 900) {
      delay(1000);
      startVerify++;
    }
    EEPROM.write(delayedStartAddr, 2);
    lcd.clear();
    lcd.print("System Started");
  } */
  
}

void loop() {

}


  
  
// ----------  clean up  ------------
  
 
  boolean switchState = digitalRead(sw1);
  if (switchState == HIGH) {
    digitalWrite(aer, HIGH);
    lcd.setCursor(0,0);
    lcd.print("Aeration Lite   ");
    menuDelay(6); // 60   // aerate for a minute
    digitalWrite(aer, LOW);
    lcd.setCursor(0,0);
    lcd.print("Idle            ");
    menuDelay(3); // 3600 wait an hour
  }
  else {
    for(int cyc = 0; cyc < 3; cyc++) {
      if(cyc > 0) {
        digitalWrite(slu, HIGH);
        lcd.setCursor(0,0);
        lcd.print("Filling         ");
        delay(3000);
        digitalWrite(slu, LOW);
        menuDelay(1); // 180
      }
      digitalWrite(aer, HIGH);
      lcd.setCursor(0,0);
      lcd.print("Aeration        ");
      delay(900); //90000
      digitalWrite(aer, LOW);
      lcd.setCursor(0,0);
      lcd.print("Idle            ");
      menuDelay(9); //900
      digitalWrite(aer, HIGH);
      lcd.setCursor(0,0);
      lcd.print("Denit          ");
      delay(3000);
      digitalWrite(aer, LOW);
      lcd.setCursor(0,0);
      lcd.print("Idle            ");
      menuDelay(9); //900
      digitalWrite(aer, HIGH);
      lcd.setCursor(0,0);
      lcd.print("Denit          ");
      delay(3000);
      digitalWrite(aer, LOW);
      lcd.setCursor(0,0);
      lcd.print("Idle            ");
      menuDelay(3); 
     }
    menuDelay(6); // 6000
    int alarmCount = 0; 
    lcd.setCursor(0,0);
    lcd.print("Denit           ");
    while(digitalRead(sw1) == LOW) {
      digitalWrite(cwp, HIGH);
      delay(1000);
      alarmCount++;
      if(alarmCount < 1800) {
        digitalWrite(light, HIGH);
        lcd.setCursor(0,1);
        lcd.print("Clear Water Err");
        break;
      }
      }
    }

// ----------  clean up  ------------

//********************************************
void menu() {
  byte buttonPressed = checkButtons();
    if (buttonPressed == but1) {
      lcd.clear();
      lcd.print(switchStr);
      for(int menuTime = 0; menuTime < 60; menuTime++) {
        int menu1 = checkButtons();
        if (menu1 == but1) {
          digitalWrite(aer, HIGH);
          lcd.clear();
          lcd.print("Aer on");
          for (int aerTimer = 0; aerTimer < 1200; aerTimer++); {
            boolean aerSwitchState = digitalRead(but1);
            if (aerSwitchState == LOW) {
              digitalWrite(aer, LOW);
              lcd.clear();
              break;
            }
            delay(500);
          }
        digitalWrite(aer, LOW);
        lcd.clear();
        delay(500);
    }
      if (buttonPressed == but2) {
          digitalWrite(slu, HIGH);
          lcd.clear();
          lcd.print("Slu on");
          for (int sluTimer = 0; sluTimer < 1200; sluTimer++); {
            boolean sluSwitchState = digitalRead(but2);
            if (sluSwitchState == LOW) {
              digitalWrite(slu, LOW);
              lcd.clear();
              break;
            }
            delay(500);
          }
        digitalWrite(slu, LOW);
        lcd.clear();
        delay(500);
    }
    
    if (buttonPressed == but3) {
          digitalWrite(cwp, HIGH);
          lcd.clear();
          lcd.print("CWP on");
          for (int cwpTimer = 0; cwpTimer < 1200; cwpTimer++); {
            boolean cwpSwitchState = digitalRead(but3);
            if (cwpSwitchState == LOW) {
              digitalWrite(cwp, LOW);
              lcd.clear();
              break;
            }
            delay(500);
          }
        digitalWrite(cwp, LOW);
        lcd.clear();
        delay(500);
    }
      }
 
}
}



//********************************************
void print8(String string, byte scrnLine) {
  lcd.setCursor(0, scrnLine);
  lcd.print(string);
}
