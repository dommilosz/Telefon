void ShowMenuAndFetchSMS() {
  ReadSMS();
  menu = MenuSMS_PRE_MENU_ID;
  sms_menu_item = 0;
}

void GenerateFields_Status(int draw_index) {
  if (draw_index < 1)return;

  String data = AT_STATUSES[draw_index - 1];
  data += ": ";
  data += (*((byte*)(&status_cache) + (draw_index - 1)));
  menus[menu].UpdateField_Txt(draw_index, data);
}

void GenerateFields_SMS(int draw_index) {
  lcd.setCursor(0, 0);
  lcd.print("         ");
  lcd.setCursor(0, 0);
  lcd.print(sms_page + 1);
  lcd.print("/");
  lcd.print(sms_pages_count);
  lcd.setCursor(0, 1);

  if (draw_index > 0 && draw_index < 8) {
    SMSStruct *sms_ptr = vsmses[(sms_page * 7) + (draw_index - 1)];
    if (sms_ptr != NULL) {
      SMSStruct sms = *sms_ptr;
      if (!sms.error) {
        menus[menu].UpdateField(draw_index, sms.phoneno.substring(0, 15),
        { []() {
            selected_sms = menus[menu].action_index;
            current_sms = vsmses[(sms_page * 7) + (menus[menu].action_index - 1)];
            menus[MenuSMS_View_MENU_ID].Show();
          }
        }, true);
        menus[menu].UpdateField_D(draw_index, sms.message.substring(0, 15), true);
      } else {
        menus[menu].fields[draw_index].valid = false;
      }
    } else {
      menus[menu].fields[draw_index].valid = false;
    }
  }
}

void SMS_NextPage() {
  if ((sms_page + 1) < sms_pages_count) {
    sms_page++;
    menus[menu].Reset();
  }
}

void SMS_PrevPage() {
  if (sms_page > 0) {
    sms_page--;
    menus[menu].Reset();
  }
}

void GenerateFields_SMS_PRE(int draw_index) {
  if (draw_index < 1)return;

  String data = "";
  switchSMS("ALL");

  if (draw_index == 1) {
    data += "ALL: [";
    data += smsCount("ALL");
    data += "]";
    menus[menu].UpdateField(draw_index, data,
    { []() {
        switchSMS("ALL");
        menus[MenuSMS_MENU_ID].Show();
      }
    });
  } else if (draw_index == 2) {
    data += "UNREAD: [";
    data += smsCount("UNREAD");
    data += "]";
    menus[menu].UpdateField(draw_index, data,
    { []() {
        switchSMS("UNREAD");
        menus[MenuSMS_MENU_ID].Show();
      }
    });
  } else if (draw_index == 3) {
    data += "READ: [";
    data += smsCount("READ");
    data += "]";
    menus[menu].UpdateField(draw_index, data,
    { []() {
        switchSMS("READ");
        menus[MenuSMS_MENU_ID].Show();
      }
    });
  } else if (draw_index == 4) {
    data += "SENT: [";
    data += smsCount("SENT");
    data += "]";
    menus[menu].UpdateField(draw_index, data,
    { []() {
        switchSMS("SENT");
        menus[MenuSMS_MENU_ID].Show();
      }
    });
  } else if (draw_index == 5) {
    data += "UNSENT: [";
    data += smsCount("UNSENT");
    data += "]";
    menus[menu].UpdateField(draw_index, data,
    { []() {
        switchSMS("UNSENT");
        menus[MenuSMS_MENU_ID].Show();
      }
    });
  } else if (draw_index == 6) {
    data += "MEMORY USAGE";
    menus[menu].UpdateField(draw_index, data,
    { []() {
        menus[MenuSMS_USAGE_MENU_ID].Show();
        FetchUsage();
      }
    });
  }
}

void GenerateFields_SMS_USAGE(int draw_index) {
  if (draw_index == 1) {
    String data = "SMS: [";
    data += MemUsage[0];
    data += "/";
    data += MemUsage[1];
    data += "]";
    menus[menu].UpdateField_Txt(draw_index, data);
  }
}

void GenerateFields_SMS_View(int draw_index) {
  SMSStruct sms = *current_sms;
  if (draw_index == 1) {
    String data = "N: ";
    data += sms.phoneno.substring(0, 15);
    menus[menu].UpdateField_Txt(draw_index, data);
  }
  if (draw_index == 2) {
    menus[menu].UpdateField_Txt(draw_index, sms.message.substring(0, 15));
  }
  if (draw_index == 3) {
    menus[menu].UpdateField_Txt(draw_index, sms.status);
  }
  if (draw_index == 4) {
    menus[menu].UpdateField_Txt(draw_index, sms.date);
  }
}

void SMSViewAction_TextView() {
  SMSStruct sms = *current_sms;
  ShowTextView(sms.message);
}

void SMSViewAction_Mark_Read() {
  SMSStruct sms = *current_sms;
  TakeATSemaphore();
  gsm.read(sms.id, true);
  ReleaseATSemaphore();
  ReadSMS();
}

void SMSViewAction_Delete() {
  SMSStruct sms = *current_sms;
  TakeATSemaphore();
  gsm.deleteOne(sms.id);
  ReleaseATSemaphore();
  menus[menu].Back();
  ReadSMS();
}

void GenerateFields_PIN(int draw_index) {
  if (draw_index == 1) {
    SMSStruct sms = *current_sms;
    String data = "";
    if (pe_error) {
      data += ("PIN: Fetching");
    } else if (pin_status == 1) {
      data += ("Enter PIN");
    } else if (pin_status == 2) {
      data += ("Enter PUK");
    }
    else if (pin_enabled) {
      data += ("Disable PIN");
    } else {
      data += ("Enable PIN");
    }

    menus[menu].UpdateField(draw_index, sms.date, PIN_Enter_PIN);
  }
}

void PIN_Change_PIN() {
  ShowInput(changePin, "Enter OLD PIN:");
}

void PIN_Enter_PIN() {
  if (pe_error) {
    lcd.print("PIN: Fetching");
  } else if (pin_status == 2) {
    ShowInput(setPin, "Enter PUK:");
  } else {
    ShowInput(setPin, "Enter PIN:");
  }
}

void PhoneBook_Launch() {
  getPhoneBook();
  menus[MenuPhoneBook_MENU_ID].Show();
}

void GenerateFields_PhoneBook(int draw_index) {
  lcd.setCursor(0, 0);
  lcd.print("         ");
  lcd.setCursor(0, 0);
  lcd.print(sms_page + 1);
  lcd.print("/");
  lcd.print(pe_pages_count);
  lcd.setCursor(0, 1);

  if (draw_index > 0 && draw_index < 8) {
    PhoneBookEntry pe = phoneBook[(sms_page * 7) + (draw_index - 1)];
    if (!pe.error) {
      if (menuItem % 2 == 0) {
        lcd.print(pe.name.substring(0, 15));
      } else {
        lcd.print(pe.phoneno.substring(0, 15));
      }

      menus[menu].UpdateField(draw_index, pe.phoneno.substring(0, 15),
      { []() {
          selected_sms = menus[menu].action_index - 1;
          current_pe = &phoneBook[(sms_page * 7) + (menus[menu].action_index - 1)];
          menus[MenuPhoneBook_View_MENU_ID].Show();
        }
      }, true);
      menus[menu].UpdateField_D(draw_index, pe.name.substring(0, 15), true);

    } else {
      menus[menu].fields[draw_index].valid = false;
    }
  }
}

void PE_NextPage() {
  if ((sms_page + 1) < pe_pages_count) {
    sms_page++;
    menus[menu].Reset();
  }
}

void PE_PrevPage() {
  if (sms_page > 0) {
    sms_page--;
    menus[menu].Reset();
  }
}

void PEViewAction_Call() {
  PhoneBookEntry pe = *current_pe;
  VoiceCall(&pe.phoneno);
}

void PEViewAction_Edit() {
  //TODO: Add edit
}

void PEViewAction_Delete() {
  PhoneBookEntry pe = *current_pe;
  TakeATSemaphore();
  gsm.deletePhoneBookEntry(pe.id);
  ReleaseATSemaphore();
  menu = MenuPhoneBook_MENU_ID;
  getPhoneBook();
}

void GenerateFields_PE_View(int draw_index) {
  PhoneBookEntry pe = *current_pe;
  if (draw_index == 0) {
    String data = "N: ";
    data += pe.phoneno.substring(0, 15);
    menus[menu].UpdateField_Txt(draw_index, data);
  }
  if (draw_index == 1) {
    menus[menu].UpdateField_Txt(draw_index, pe.name.substring(0, 15));
  }
  if (draw_index == 2) {
    String data = "Type: ";
    data += pe.type;
    menus[menu].UpdateField_Txt(draw_index, data);
  }
}
