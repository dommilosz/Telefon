void UpdateLED() {
  if (AT_STATUS == STATUS_NOAT) {
    SetLEDColor(255, 0, 0);
    return;
  }
  if (AT_STATUS == STATUS_ERROR) {
    SetLEDColor(255, 0, 255);
    return;
  }
  if (AT_STATUS == STATUS_OTHER) {
    SetLEDColor(255, 255, 0);
    return;
  }
  if (AT_STATUS == STATUS_UNREG) {
    SetLEDColor(255, 0, 255);
    return;
  }

  if (AT_STATUS == STATUS_BOARD) {
    SetLEDColor(0, 255, 255);
    return;
  }

  if (AT_STATUS == STATUS_CALL) {
    if ((loopI % 300) > 150) {
      SetLEDColor(255, 0, 0);
    } else {
      SetLEDColor(255, 0, 0);
    }
    return;
  }



  if (AT_STATUS == STATUS_OK) {
    SetLEDColor(0, 255, 0);
    return;
  }
}

void DrawCurrentMenu() {

  lcd.clear();
  lcd.home();
  lcd.print("ATos");
  lcd.setCursor(5, 0);
  lcd.print("#");
  lcd.print(MENUS[menu]);
  lcd.setCursor(11, 0);
  lcd.print(STATUS_STR[AT_STATUS]);
  lcd.setCursor(0, 1);

  if (menu == 0) {
    if (board_buffi) {
      lcd.print(BufferToString(board_buffor, board_buffi));
      lcd.print(" : ");
      lcd.print(millis() - lastInt);
    } else {
      lcd.print(LastMenuMsg);
    }
  }

  if (menu == MenuSelect_MENU_ID) {
    MenuSelect_Draw();
  }
  if (menu == MenuStatus_MENU_ID) {
    MenuStatus_Draw();
  }
  if (menu == MenuTimings_MENU_ID) {
    MenuTimings_Draw();
  }
  if (menu == MenuSMS_MENU_ID) {
    MenuSMS_Draw();
  }
  if (menu == MenuSMS_PRE_MENU_ID) {
    MenuSMS_PRE_Draw();
  }

  if (menu == MenuSMS_View_MENU_ID) {
    MenuSMS_View_Draw();
  }

  if (menu == MenuSMS_TV_MENU_ID) {
    MenuSMS_TV_Draw();
  }
}
