//
//MenuPhoneBook_Pre
//

void MenuPhoneBook_Pre_Draw() {
  lcd.setCursor(0, 1);
  if (menuItem >= 3) {
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
    lcd.print("New [WIP]");
  }
  if (menuItem == 2) {
    lcd.print(menuItem);
    lcd.print(": ");
    lcd.print("PhoneBook");
  }
}

void MenuPhoneBook_Pre_Action(byte item) {
  if (item == 0) {
    menu = MenuSelect_MENU_ID;
  }
  if (item == 1) {
    //menu = 
  }
  if (item == 2) {
    menu = MenuPhoneBook_MENU_ID;
    InvokeOnWorker(getPhoneBook);
  }
}

//
//MenuPhoneBook
//

void MenuPhoneBook_Draw() {
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
    lcd.print(pe_pages_count);
    lcd.setCursor(0, 1);
  }
  if (sms_menu_item == 0) {
    lcd.print(sms_menu_item);
    lcd.print(": ");
    lcd.print(MenuStdExit_MENU[sms_menu_item]);
  }

  if (sms_menu_item > 0 && sms_menu_item < 8) {
    PhoneBookEntry pe = phoneBook[(sms_page * 7) + (sms_menu_item - 1)];
    if (!pe.error) {
      lcd.print(sms_menu_item);
      lcd.print(":");
      if (menuItem % 2 == 0) {
        lcd.print(pe.name.substring(0, 15));
      } else {
        lcd.print(pe.phoneno.substring(0, 15));
      }

    } else {
      sms_menu_item = 8;
      MenuPhoneBook_Draw();
    }
  }

  if (sms_menu_item == 8) {
    lcd.print("8: Previous Page");
  }
  if (sms_menu_item == 9) {
    lcd.print("9: Next Page");
  }
}

void MenuPhoneBook_Action(byte item) {
  if (item == 0) {
    menu = MenuPhoneBook_Pre_MENU_ID;
  }
  if (item > 0 && item < 8) {
    selected_sms = item - 1;
    menuItem = 0;
    loopI500 = 0;
    menu = MenuPhoneBook_View_MENU_ID;
    current_pe = &phoneBook[(sms_page * 7) + (item - 1)];
  }
  if (item == 8) {
    if (sms_page > 0) {
      sms_page--;
      menuItem = 0;
      sms_menu_item = 0;
    }
  }
  if (item == 9) {
    if ((sms_page + 1) < pe_pages_count) {
      sms_page++;
      menuItem = 0;
      sms_menu_item = 0;
    }
  }
}

//
//MenuSMS_View
//

void MenuPhoneBook_View_Draw() {
  lcd.setCursor(0, 1);
  if (invokePointer > 0) {
    lcd.print("Loading...");
    menuItem = 0;
    sms_menu_item = 0;
    return;
  }
  PhoneBookEntry pe = *current_pe;
  if (menuItem >= 6) {
    menuItem = 0;
  }
  if (menuItem == 0) {
    lcd.print("N: ");
    lcd.print(pe.phoneno.substring(0, 15));
  }
  if (menuItem == 1) {
    lcd.print(pe.name.substring(0, 15));
  }
  if (menuItem == 2) {
    lcd.print("Type: ");
    lcd.print(pe.type);
  }

  if (menuItem == 3) {
    lcd.print("3: Call");
  }

  if (menuItem == 4) {
    lcd.print("4: Edit WIP");
  }

  if (menuItem == 5) {
    lcd.print("5: Delete");
  }

}

void MenuPhoneBook_View_Action(byte item) {
  if (item == 0) {
    menu = MenuPhoneBook_MENU_ID;
  }
  PhoneBookEntry pe = *current_pe;

  if (item == 3) {
    VoiceCall(&pe.phoneno);
    menu = 0;
    loopI500 = 0;
  }

  if (item == 4) {
    
  }

  if (item == 5) {
    gsm.deletePhoneBookEntry(pe.id);
    menu = MenuPhoneBook_MENU_ID;
    InvokeOnWorker(getPhoneBook);
  }
}
