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
      if (board_buffi >= 3) {
        lcd.print(millis() - lastInt);
      }
    } else {
      if (LastMenuMsg.length() > 0) {
        lcd.print(LastMenuMsg);
      } else {
        lcd.print(op_name);
      }
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

  if (menu == Menu_TV_MENU_ID) {
    Menu_TV_Draw();
  }

  if (menu == MenuSMS_USAGE_MENU_ID) {
    MenuSMS_USAGE_Draw();
  }

  if (menu == MenuSettings_MENU_ID) {
    MenuSettings_Draw();
  }

  if (menu == MenuPIN_MENU_ID) {
    MenuPIN_Draw();
  }

  if (menu == MenuInput_MENU_ID) {
    MenuInput_Draw();
  }

  if (menu == MenuConfirm_MENU_ID) {
    Menu_Confirm_Draw();
  }

  if (menu == Menu_TXTDelay_MENU_ID) {
    Menu_TXTDelay_Draw();
  }

  if (menu == MenuPhoneBook_Pre_MENU_ID) {
    MenuPhoneBook_Pre_Draw();
  }

  if (menu == MenuPhoneBook_MENU_ID) {
    MenuPhoneBook_Draw();
  }

  if (menu == MenuPhoneBook_View_MENU_ID) {
    MenuPhoneBook_View_Draw();
  }
}
