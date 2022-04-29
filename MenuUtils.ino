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
    SetLEDColor(0, 0, 255);
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
  lcd.print(menus[menu].code);
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

  } else {
    menus[menu].Draw();
  }
}

void DrawCurrentMenuGC() {
  String header = "ATos #";
  header += menus[menu].code;
  header += "  ";
  header += STATUS_STR[AT_STATUS];
  SetAsset(11, &header);

  if (menu == 0) {
    String asset = "";
    String txt = "Press 0 to enter menu!";
    SetAsset(3, &txt);
    
    SetAsset(2, &asset);
    SetAsset(4, &asset);
    SetAsset(5, &asset);
    SetAsset(6, &asset);
    SetAsset(7, &asset);
    SetAsset(8, &asset);
    SetAsset(9, &asset);
    SetAsset(10, &asset);
    if (board_buffi) {
      asset += BufferToString(board_buffor, board_buffi);
      asset += " : ";
      if (board_buffi >= 3) {
        asset += (millis() - lastInt);
      }
    } else {
      if (LastMenuMsg.length() > 0) {
        asset = LastMenuMsg;
      } else {
        asset = op_name;
      }
    }
    SetAsset(1, &asset);
  } else {
    menus[menu].DrawGC();
  }


}
