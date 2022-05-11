int smsCount(String status) {
  int count;
  for (int i = 0; i < 128; i++) {
    if (!smses[i].error) {
      if (status == "ALL") {
        count ++;
      } else {
        if (smses[i].status == status) {
          count ++;
        }
      }

    }
  }
  return count;
}

void switchSMS(String status) {
  l_status = status;
  sms_page = 0;
  int i2 = 0;
  for (int i = 0; i < 128; i++) {
    if (status == "ALL") {
      vsmses[i] = &smses[i];
    } else {
      vsmses[i] = NULL;
      if (!smses[i].error) {
        if (smses[i].status == status) {
          vsmses[i2] = &smses[i];
          i2 ++;
        }
      }
    }

  }

  int c = smsCount(status);
  SMSStruct *vsmses_tmp[c];
  for (int i = 0; i < c; i++) {
    vsmses_tmp[i] = vsmses[i];
  }
  for (int i = 0; i < c; i++) {
    vsmses[i] = vsmses_tmp[(c - 1) - i];
  }

  sms_count = c;
  sms_pages_count = ((sms_count - 1) / 7) + 1;
}

SMSStruct *_GetSms(int index, bool read = false) {
  AssertCore(0);
  SMSStruct sms = gsm.read(index, read);
  memcpy(task_mem_buff, &sms, sizeof(SMSStruct));

  return (SMSStruct*)task_mem_buff;
}

SMSStruct GetSms(int index, bool read = false) {
  SMSStruct *sms = (SMSStruct*)DelegateTask((ptrRetFunc)_GetSms);
  return *sms;
}

void *_ReadSMS() {
  AssertCore(0);
  sms_count = 0;
  gsm.setTextMode(true);

  for (int i = 0; i < 128; i++) {
    smses[i].error = true;
  }

  gsm.list(Cb_SMS, false);

  sms_pages_count = ((sms_count - 1) / 7) + 1;
  switchSMS(l_status);
  return NULL;
}

void ReadSMS() {
  DelegateTask(_ReadSMS);
}


void Cb_SMS(SMSStruct sms) {
  smses[sms_count] = sms;
  sms_count ++;
}

String sms_number;
String sms_text;

void SMS_Phonebook(PhoneBookEntry *pe) {
  menus[MenuSMS_New_MENU_ID].Show();
  NewSMS_ChangeNumber(pe->phoneno);
  NewSMS_ChangeText("");
}

void NewSMS_Show() {
  menus[MenuSMS_New_MENU_ID].Show();
  NewSMS_ChangeNumber("");
  NewSMS_ChangeText("");
}

void NewSMS_ChangeNumber() {
  ShowInput(NewSMS_ChangeNumber, "Enter NUMBER: ");
}

void NewSMS_ChangeNumber(String number) {
  sms_number = number;
  String d = "Numb: ";
  d += number;
  menus[menu].UpdateField_Txt(1, d);
}

void NewSMS_ChangeText() {
  ShowInput(NewSMS_ChangeText, "Enter Text: ", true, true);
}

void NewSMS_ChangeText(String text) {
  sms_text = text;
  String d = "Text: ";
  d += text;
  menus[menu].UpdateField_Txt(2, d);
}

void NewSMS_TextView() {
  ShowTextView(sms_text);
}

void NewSMS_Send() {
  NewSMS_Send_Invoke();
}

char *__number2;
char *__sms_text;
void *_SendSMS() {
  AssertCore(0);
  if (gsm.send(__number2, __sms_text)) {
    Menu_Back();
    ShowTXTD("SENT!", 4);
  } else {
    ShowTXTD("ERROR!", 4);
  }
  return NULL;
}

void SendSMS(char *number, char *text){
  DelegateTask(_SendSMS);
  __number2 = number;
  __sms_text = text;
}

void NewSMS_Send_Invoke() {
  if (sms_number.length() > 2 && sms_text.length() > 0) {
    char number[sms_number.length()];
    char text[sms_text.length()];
    sms_number.toCharArray(number, sms_number.length() + 1);
    sms_text.toCharArray(text, sms_text.length() + 1);
    SendSMS(number, text);
  } else {
    if (sms_number.length() <= 2) {
      ShowTXTD("SHORT NUMBER!", 4);
    } else {
      ShowTXTD("MESSAGE NEEDED!", 4);
    }
  }
}
