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
  input.toCharArray(c, input.length() + 1);

  Serial.println(c);
  Serial.println(input);

  
  if (pin_status == 1) {
    TakeATSemaphore();
    if (gsm.enterPinCode(c)) {
      ShowTXTD("SUCCESS", 2);
    } else {
      ShowTXTD("ERROR", 2);
    }
    ReleaseATSemaphore();
  }
  if (pin_status == 2) {
    puk[input.length()] = 0;
    input.toCharArray(puk, input.length() + 1);
    ShowInput(UnlockPUK, "Enter new pin:");
    return;
  }
  else if (pin_enabled) {
    TakeATSemaphore();
    if (gsm.disablePinCode(c)) {
      ShowTXTD("SUCCESS", 2);
    } else {
      ShowTXTD("ERROR", 2);
    }
    ReleaseATSemaphore();
  } else {
    TakeATSemaphore();
    if (gsm.enablePinCode(c)) {
      ShowTXTD("SUCCESS", 2);
    } else {
      ShowTXTD("ERROR", 2);
    }
    ReleaseATSemaphore();
  }
  lcd.setCursor(0, 1);
  lcd.print("LOADING...          ");
  FetchPIN();
}

void UnlockPUK(String newpin) {
  char c[8];
  c[newpin.length()] = 0;
  newpin.toCharArray(c, newpin.length() + 1);

  TakeATSemaphore();
  if (gsm.enterPukCode(puk, c)) {
    ShowTXTD("SUCCESS", 2);
  } else {
    ShowTXTD("ERROR", 2);
  }
  ReleaseATSemaphore();
}

void changePin(String pin) {
  puk[pin.length()] = 0;
  pin.toCharArray(puk, pin.length() + 1);
  ShowInput(changePin2, "Enter new pin:");
  return;
}

void changePin2(String newpin) {
  char c[8];
  c[newpin.length()] = 0;
  newpin.toCharArray(c, newpin.length() + 1);
  TakeATSemaphore();
  if (gsm.changePinCode(puk, c)) {
    lcd.print("SUCCESS");
  } else {
    lcd.print("ERROR");
  }
  ReleaseATSemaphore();
}
