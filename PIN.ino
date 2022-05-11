
char *__codepin;
char puk[8];

void *_EnterPinCode() {
  AssertCore(0);
  if (gsm.enterPinCode(__codepin)) {
    ShowTXTD("SUCCESS", 2);
  } else {
    ShowTXTD("ERROR", 2);
  }
  return NULL;
}

void EnterPinCode(char *c) {
  __codepin = c;
  DelegateTask(_EnterPinCode);
}

void *_DisablePinCode() {
  AssertCore(0);
  if (gsm.disablePinCode(__codepin)) {
    ShowTXTD("SUCCESS", 2);
  } else {
    ShowTXTD("ERROR", 2);
  }
  return NULL;
}

void *_EnablePinCode() {
  AssertCore(0);
  if (gsm.enablePinCode(__codepin)) {
    ShowTXTD("SUCCESS", 2);
  } else {
    ShowTXTD("ERROR", 2);
  }
  return NULL;
}

void EnablePinCode(char *c) {
  __codepin = c;
  DelegateTask(_EnablePinCode);
}

void DisablePinCode(char *c) {
  __codepin = c;
  DelegateTask(_DisablePinCode);
}

void *_EnterPukCode() {
  AssertCore(0);
  if (gsm.enterPukCode(puk, __codepin)) {
    ShowTXTD("SUCCESS", 2);
  } else {
    ShowTXTD("ERROR", 2);
  }
  return NULL;
}

void *_ChangePinCode() {
  AssertCore(0);
  if (gsm.changePinCode(puk, __codepin)) {
    ShowTXTD("SUCCESS", 2);
  } else {
    ShowTXTD("ERROR", 2);
  }
  return NULL;
}

void EnterPukCode(char *c) {
  __codepin = c;
  DelegateTask(_EnterPukCode);
}

void ChangePinCode(char *c) {
  __codepin = c;
  DelegateTask(_ChangePinCode);
}


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
    EnterPinCode(c);
  }
  if (pin_status == 2) {
    puk[input.length()] = 0;
    input.toCharArray(puk, input.length() + 1);
    ShowInput(UnlockPUK, "Enter new pin:");
    return;
  }
  else if (pin_enabled) {
    DisablePinCode(c);
  } else {
    EnablePinCode(c);
  }
  lcd.setCursor(0, 1);
  lcd.print("LOADING...          ");
  FetchPIN();
}

void UnlockPUK(String newpin) {
  char c[8];
  c[newpin.length()] = 0;
  newpin.toCharArray(c, newpin.length() + 1);

  EnterPukCode(c);
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

  ChangePinCode(c);
}
