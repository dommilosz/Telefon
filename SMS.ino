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
    vsmses[i] = vsmses_tmp[(c-1) - i];
  }

  sms_count = c;
  sms_pages_count = ((sms_count - 1) / 7) + 1;
}

SMSStruct GetSms(int index, bool read = false) {
  SMSStruct sms = gsm.read(index, read);
  return sms;
}

void ReadSMS() {
  sms_count = 0;
  gsm.setTextMode(true);

  for (int i = 0; i < 128; i++) {
    smses[i].error = true;
  }

  gsm.list(Cb_SMS, false);

  Serial.println("\n[SMSCount:]\n");
  sms_pages_count = ((sms_count - 1) / 7) + 1;
  Serial.print(sms_count);

  switchSMS(l_status);
}

void Cb_SMS(SMSStruct sms) {
  smses[sms_count] = sms;
  sms_count ++;
}
