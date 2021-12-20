bool TestAT() {
  return Command("AT") == "OK";
}

byte status_cache[7];
byte *GetStatus() {
  String reg = Command("AT+CIND?");
  reg = getValue(reg, ':', 1);
  status_cache[0] = (getValue(reg, ',', 0).toInt());
  status_cache[1] = (getValue(reg, ',', 1).toInt());
  status_cache[2] = (getValue(reg, ',', 2).toInt());
  status_cache[3] = (getValue(reg, ',', 3).toInt());
  status_cache[4] = (getValue(reg, ',', 4).toInt());
  status_cache[5] = (getValue(reg, ',', 5).toInt());
  status_cache[6] = (getValue(reg, ',', 6).toInt());

  return status_cache;
}

void VoiceCall(String * number) {
  String cmd = "ATD+48";
  cmd += *number;
  cmd += ";";
  String resp = Command(cmd);
  if (resp != "OK") {
    LastMenuMsg = "ERR: " + resp;
  }
}

void CheckConnection() {
  long current = millis();
  long diffOK = current - lastOK;
  long diffAT = current - lastAT;
  long diffCREG = current - lastCREG;

  if (diffOK > 3000) {
    AT_STATUS = STATUS_NOAT;
    if (diffAT > 500) {
      Serial2.begin(9600);
      if (TestAT()) {
        lastOK = millis();
      }
      lastAT = millis();
    }
  } else if (diffOK > 1000) {
    if (diffAT > 500) {
      if (Command("AT") == "OK") {
        lastOK = millis();
      }
      lastAT = millis();
    }
  }

  if (diffAT > 500 && diffCREG > 5000) {
    //+CIND: 5,4,1,1,1,0,0 - call
    //+CIND: 5,4,1,1,0,0,0 - no call
    //+CIND: 5,0,0,0,0,0,1 - no reg
    //+CIND:("battchg",(0-5)), ("signal",(0-5)), ("service",(0,1)), ("message",(0,1)),("call",(0,1)), ("roam",(0,1)), ("smsfull",(0,1))
    byte *status = GetStatus();

    last_reg_status = STATUS_OK;
    if (!status[2]) {
      AT_STATUS = STATUS_UNREG;
      last_reg_status = STATUS_UNREG;
    }
    if (status[4]) {
      AT_STATUS = STATUS_CALL;
      last_reg_status = STATUS_CALL;
    }

    lastCREG = millis();
  } else if (AT_STATUS == STATUS_OK && last_reg_status == STATUS_UNREG) {
    AT_STATUS = STATUS_UNREG;
  }
}

String Command(String cmd) {
  Serial.print("\n> " + cmd + "\n");
  FlushSerial2();
  Serial2.print(cmd + "\r");
  return ReadResponse();
}

void TextMode(){
  Command("AT+CMGF=1");
}

SMSStruct smses[7];

SMSStruct GetSms(int index,bool read = false){
  TextMode();
  String cmd = "AT+CMGR=";
  cmd += index;
  cmd += ",";
  cmd += (byte)(!read);
  String reg = Command(cmd);
  SMSStruct sms;
  sms.state = (getValue(reg, '"', 1).toInt());
  sms.address = (getValue(reg, '"', 3).toInt());
  sms.idk = (getValue(reg, '"', 5).toInt());
  sms.date = (getValue(reg, '"', 7).toInt());
  sms.data = Serial2.readStringUntil(10);
  sms.id = index;
  Serial.print("SMS:");
  Serial.print(sms.state);
  Serial.print("  ");
  Serial.print(sms.address);
  Serial.print("  ");
  Serial.print(sms.data);
  Serial.print("  ");
  Serial.print(reg);
  Serial.print("END");
  return sms;
}

void ReadSMSPage(){
  for(int i = 0;i<7;i++){
    smses[i] = GetSms(i+(sms_page*7)+1);
  }
}
