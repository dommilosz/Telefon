const char *MenuStdExit_MENU[1] = {"EXIT"};

const byte MenuSelect_MENU_ID = 1;
const byte MenuSelect_MENU_LEN = 4;
const char *MenuSelect_MENU[MenuSelect_MENU_LEN] = {"EXIT","STATUS","TIMINGS","SMS"};

const byte MenuStatus_MENU_ID = 2;
const byte MenuStatus_MENU_LEN = 1;

const byte MenuTimings_MENU_ID = 3;
const byte MenuSMS_MENU_ID = 4;

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
    InvokeOnWorker(ReadSMSPage);
    menu = MenuSMS_MENU_ID;
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
    lcd.print(status_cache[menuItem - 1]);
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

//
//MenuTimings
//

void MenuSMS_Draw() {
  lcd.setCursor(0, 1);
  if (menuItem == 0) {
    lcd.print(menuItem);
    lcd.print(": ");
    lcd.print(MenuStdExit_MENU[menuItem]);
  }
  
  if (menuItem > 0 && menuItem < 8) {
    lcd.print(smses[menuItem-1].id);
    lcd.print(":");
    lcd.print(smses[menuItem-1].address);
    lcd.print(":");
    lcd.print(smses[menuItem-1].data);
  }
  
  if (menuItem == 8) {
    lcd.print("8: Previous Page");
  }
  if (menuItem == 9) {
    lcd.print("9: Next Page");
  }
}

void MenuSMS_Action(byte item) {
  if (item == 0) {
    menu = MenuSelect_MENU_ID;
  }
}
