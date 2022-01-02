

//
//MenuSelect
//

void MenuSelect_Draw() {
  lcd.setCursor(0, 1);
  if (menuItem >= MenuSelect_MENU_LEN) {
    menuItem = 0;
  }
  lcd.print(menuItem);
  lcd.print(": ");
  lcd.print(MenuSelect_MENU[menuItem]);
}

void MenuSelect_Action(byte item) {
  if (item == 0) {
    menu = 0;
  }
  if (item == 1) {
    menu = MenuStatus_MENU_ID;
  }
  if (item == 2) {
    menu = MenuTimings_MENU_ID;
  }
  if (item == 3) {
    InvokeOnWorker(ReadSMS);
    menu = MenuSMS_PRE_MENU_ID;
    menuItem = 0;
    sms_menu_item = 0;
    loopI500 = 0;
  }
  if (item == 4) {
    menu = MenuSettings_MENU_ID;
  }
  if (item == 5) {
    menu = MenuPhoneBook_Pre_MENU_ID;
  }
}

//
//MenuStatus
//

void MenuStatus_Draw() {
  lcd.setCursor(0, 1);
  if (menuItem >= 8) {
    menuItem = 0;
  }
  if (menuItem == 0) {
    lcd.print(menuItem);
    lcd.print(": ");
    lcd.print(MenuStdExit_MENU[menuItem]);
  } else {
    lcd.print(AT_STATUSES[menuItem - 1]);
    lcd.print(": ");
    lcd.print((*((byte*)(&status_cache) + (menuItem - 1))));
  }
}

void MenuStatus_Action(byte item) {
  if (item == 0) {
    menu = MenuSelect_MENU_ID;
  }
}

//
//MenuTimings
//

void MenuTimings_Draw() {
  lcd.setCursor(0, 1);
  if (menuItem >= 4) {
    menuItem = 0;
  }
  if (menuItem == 0) {
    lcd.print(menuItem);
    lcd.print(": ");
    lcd.print(MenuStdExit_MENU[menuItem]);
  }
  if (menuItem == 1) {
    lcd.print("Last 5:");
    lcd.print(maxTimings[0]);
    lcd.print("ms");
  }
  if (menuItem == 2) {
    lcd.print("Last 500:");
    lcd.print(maxTimings[1]);
    lcd.print("ms");
  }
  if (menuItem == 3) {
    lcd.print("Last 1000:");
    lcd.print(maxTimings[2]);
    lcd.print("ms");
  }
}

void MenuTimings_Action(byte item) {
  if (item == 0) {
    menu = MenuSelect_MENU_ID;
  }
}
