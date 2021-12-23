

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
    menu = MenuSMS_PRE_MENU_ID;
    menuItem = 0;
    sms_menu_item = 0;
    loopI500 = 0;
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

//
//MenuSMS_Pre
//

void MenuSMS_PRE_Draw() {
  lcd.setCursor(0, 1);
  if (invokePointer > 0) {
    lcd.print("Loading...");
    menuItem = 0;
    sms_menu_item = 0;
    return;
  }

  if (menuItem >= 6) {
    menuItem = 0;
  }
  if (menuItem == 0) {
    lcd.print(menuItem);
    lcd.print(": ");
    lcd.print(MenuStdExit_MENU[menuItem]);
  }
  if (menuItem == 1) {
    lcd.print(menuItem);
    lcd.print(": ALL: [");
    lcd.print(smsCount("ALL"));
    lcd.print("]");
  }
  if (menuItem == 2) {
    lcd.print(menuItem);
    lcd.print(": UNREAD: [");
    lcd.print(smsCount("UNREAD"));
    lcd.print("]");
  }
  if (menuItem == 3) {
    lcd.print(menuItem);
    lcd.print(": READ: [");
    lcd.print(smsCount("READ"));
    lcd.print("]");
  }
  if (menuItem == 4) {
    lcd.print(menuItem);
    lcd.print(": SENT: [");
    lcd.print(smsCount("SENT"));
    lcd.print("]");
  }
  if (menuItem == 5) {
    lcd.print(menuItem);
    lcd.print(": UNSENT: [");
    lcd.print(smsCount("UNSENT"));
    lcd.print("]");
  }
}

void MenuSMS_PRE_Action(byte item) {
  if (item == 0) {
    menu = MenuSelect_MENU_ID;
  }
  sms_page = 0;
  if (item == 1) {
    switchSMS("ALL");
    menu = MenuSMS_MENU_ID;
    menuItem = 0;
    sms_menu_item = 0;
    loopI500 = 0;
  }
  if (item == 2) {
    switchSMS("UNREAD");
    menu = MenuSMS_MENU_ID;
    menuItem = 0;
    sms_menu_item = 0;
    loopI500 = 0;
  }
  if (item == 3) {
    switchSMS("READ");
    menu = MenuSMS_MENU_ID;
    menuItem = 0;
    sms_menu_item = 0;
    loopI500 = 0;
  }
  if (item == 4) {
    switchSMS("SENT");
    menu = MenuSMS_MENU_ID;
    menuItem = 0;
    sms_menu_item = 0;
    loopI500 = 0;
  }
  if (item == 5) {
    switchSMS("UNSENT");
    menu = MenuSMS_MENU_ID;
    menuItem = 0;
    sms_menu_item = 0;
    loopI500 = 0;
  }
}

//
//MenuSMS
//

void MenuSMS_Draw() {
  lcd.setCursor(0, 1);
  if (invokePointer > 0) {
    lcd.print("Loading...");
    menuItem = 0;
    sms_menu_item = 0;
    return;
  }


  if (menuItem > 1) {
    menuItem = 0;
    sms_menu_item ++;
  }
  if (sms_menu_item > 9) {
    sms_menu_item = 0;
  }

  if (loopI500 % 2 == 0) {
    lcd.setCursor(5, 0);
    lcd.print("     ");
    lcd.setCursor(5, 0);
    lcd.print(sms_page + 1);
    lcd.print("/");
    lcd.print(sms_pages_count);
    lcd.setCursor(0, 1);
  }
  if (sms_menu_item == 0) {
    lcd.print(sms_menu_item);
    lcd.print(": ");
    lcd.print(MenuStdExit_MENU[sms_menu_item]);
  }

  if (sms_menu_item > 0 && sms_menu_item < 8) {
    SMSStruct *sms_ptr = vsmses[(sms_page * 7) + (sms_menu_item - 1)];
    if (sms_ptr != NULL) {
      SMSStruct sms = *sms_ptr;
      if (!sms.error) {
        lcd.print(sms_menu_item);
        lcd.print(":");
        if (menuItem % 2 == 0) {
          lcd.print(sms.phoneno.substring(0, 15));
        } else {
          lcd.print(sms.message.substring(0, 15));
        }

      } else {
        sms_menu_item = 8;
        MenuSMS_Draw();
      }
    } else {
      sms_menu_item = 8;
      MenuSMS_Draw();
    }


  }



  if (sms_menu_item == 8) {
    lcd.print("8: Previous Page");
  }
  if (sms_menu_item == 9) {
    lcd.print("9: Next Page");
  }
}

void MenuSMS_Action(byte item) {
  if (item == 0) {
    menu = MenuSMS_PRE_MENU_ID;
  }
  if (item > 0 && item < 8) {
    selected_sms = item - 1;
    menuItem = 0;
    loopI500 = 0;
    menu = MenuSMS_View_MENU_ID;
    current_sms = vsmses[(sms_page * 7) + (item - 1)];
  }
  if (item == 8) {
    if (sms_page > 0) {
      sms_page--;
      menuItem = 0;
      sms_menu_item = 0;
    }
  }
  if (item == 9) {
    if ((sms_page + 1) < sms_pages_count) {
      sms_page++;
      menuItem = 0;
      sms_menu_item = 0;
    }
  }
}

//
//MenuSMS_View
//

void MenuSMS_View_Draw() {
  lcd.setCursor(0, 1);
  if (invokePointer > 0) {
    lcd.print("Loading...");
    menuItem = 0;
    sms_menu_item = 0;
    return;
  }
  SMSStruct sms = *current_sms;
  if (menuItem >= 7) {
    menuItem = 0;
  }
  if (menuItem == 0) {
    lcd.print("N: ");
    lcd.print(sms.phoneno.substring(0, 15));
  }
  if (menuItem == 1) {
    lcd.print(sms.message.substring(0, 15));
  }
  if (menuItem == 2) {
    lcd.print(sms.status);
  }
  if (menuItem == 3) {
    lcd.print(sms.date);
  }

  if (menuItem == 4) {
    lcd.print("4: Text View");
  }

  if (menuItem == 5) {
    lcd.print("5: Mark as READ");
  }

  if (menuItem == 6) {
    lcd.print("6: Delete");
  }

}

void MenuSMS_View_Action(byte item) {
  if (item == 0) {
    menu = MenuSMS_MENU_ID;
  }
  SMSStruct sms = *current_sms;

  if (item == 4) {
    menu = MenuSMS_TV_MENU_ID;
    loopI500 = 0;
  }

  if (item == 5) {
    gsm.read(sms.id,true);
    InvokeOnWorker(ReadSMSPage);
  }
  
  if (item == 6) {
    gsm.deleteOne(sms.id);
    menu = MenuSMS_MENU_ID;
    InvokeOnWorker(ReadSMSPage);
  }
}

//
//MenuSMS_TV
//

void MenuSMS_TV_Draw() {
  lcd.setCursor(0, 1);
  if (invokePointer > 0) {
    lcd.print("Loading...");
    menuItem = 0;
    sms_menu_item = 0;
    loopI500 = 0;
    return;
  }
  SMSStruct sms = *current_sms;
  if(sms.message.length()>16){
    String msg = sms.message.substring(loopI500);
    if(msg.length()<14){
      loopI500 = 0;
    }
    lcd.print(msg);
  }else{
    lcd.print(sms.message);
  }
  

}

void MenuSMS_TV_Action(byte item) {
  if (item == 0) {
    menu = MenuSMS_View_MENU_ID;
  }
}
