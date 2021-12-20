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
    }else{
      lcd.print(LastMenuMsg);
    }
  }

  if (menu == 1) {
    MenuSelect_Draw();
  }
  if (menu == 2) {
    MenuStatus_Draw();
  }
  if (menu == 3) {
    MenuTimings_Draw();
  }
  if (menu == 4) {
    MenuSMS_Draw();
  }
}
