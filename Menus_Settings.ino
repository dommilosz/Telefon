//
//MenuSettings
//

void MenuSettings_Draw() {
  lcd.setCursor(0, 1);

  if (menuItem >= 2) {
    menuItem = 0;
  }

  if (menuItem == 0) {
    lcd.print(menuItem);
    lcd.print(": ");
    lcd.print(MenuStdExit_MENU[menuItem]);
  }

  if (menuItem == 1) {
    lcd.print(menuItem);
    lcd.print(": ");
    lcd.print("PIN");
  }


}

void MenuSettings_Action(byte item) {
  if (item == 0) {
    menu = MenuSelect_MENU_ID;
  }
  if (item == 1) {
    menu = MenuPIN_MENU_ID;
  }
}

//
//MenuPIN
//

void MenuPIN_Draw() {
  lcd.setCursor(0, 1);

  if (menuItem >= 2) {
    menuItem = 0;
  }

  if (menuItem == 0) {
    lcd.print(menuItem);
    lcd.print(": ");
    lcd.print(MenuStdExit_MENU[menuItem]);
  }

  if (menuItem == 1) {
    lcd.print(menuItem);
    lcd.print(": ");
    if (pin_status == 1) {
      lcd.print("Enter PIN");
    } else if (pin_status == 2) {
      lcd.print("Enter PUK");
    }
    else if (pin_enabled) {
      lcd.print("Disable PIN");
    } else {
      lcd.print("Enable PIN");
    }
  }

  if (menuItem == 2) {
    lcd.print(menuItem);
    lcd.print(": ");
    lcd.print("Change PIN");
  }


}

void MenuPIN_Action(byte item) {
  if (item == 0) {
    menu = MenuSettings_MENU_ID;
  }
  if (item == 1) {
    if (pin_status == 2){
      ShowInput(setPin,"Enter PUK:");
    }else{
      ShowInput(setPin,"Enter PIN:");
    }
    
  }
  if (item == 2) {
    ShowInput(changePin,"Enter OLD PIN:");
  }
}

char puk[8];
void setPin(String input) {
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  if (input.length() > 8) {
    ShowTXTD("Too Long PIN", 2);
    return;
  }
  char c[8];
  c[input.length()] = 0;
  input.toCharArray(c, input.length()+1);

  Serial.println(c);
  Serial.println(input);

  if (pin_status == 1) {
    if (gsm.enterPinCode(c)) {
      ShowTXTD("SUCCESS", 2);
    } else {
      ShowTXTD("ERROR", 2);
    }
  }
  if (pin_status == 2) {
    puk[input.length()] = 0;
    input.toCharArray(puk, input.length()+1);
    ShowInput(UnlockPUK, "Enter new pin:");
    return;
  }
  else if (pin_enabled) {
    if (gsm.disablePinCode(c)) {
      ShowTXTD("SUCCESS", 2);
    } else {
      ShowTXTD("ERROR", 2);
    }
  } else {
    if (gsm.enablePinCode(c)) {
      ShowTXTD("SUCCESS", 2);
    } else {
      ShowTXTD("ERROR", 2);
    }
  }
  lcd.setCursor(0, 1);
  lcd.print("LOADING...          ");
  FetchPIN();
}

void UnlockPUK(String newpin) {
  char c[8];
  c[newpin.length()] = 0;
  newpin.toCharArray(c, newpin.length()+1);

  if (gsm.enterPukCode(puk, c)) {
    ShowTXTD("SUCCESS",2);
  } else {
    ShowTXTD("ERROR",2);
  }
}

void changePin(String pin) {
  puk[pin.length()] = 0;
  pin.toCharArray(puk, pin.length()+1);
  ShowInput(changePin2, "Enter new pin:");
  return;
}

void changePin2(String newpin) {
  char c[8];
  c[newpin.length()] = 0;
  newpin.toCharArray(c, newpin.length()+1);
  if (gsm.changePinCode(puk, c)) {
    lcd.print("SUCCESS");
  } else {
    lcd.print("ERROR");
  }
}
