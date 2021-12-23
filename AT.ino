bool TestAT() {
  return gsm.checkConnection(1);
}

GSMStatus status_cache;

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

  if (diffOK > 15000) {
    AT_STATUS = STATUS_NOAT;
    if (diffAT > 500) {
      Serial2.begin(9600);
      if (TestAT()) {
        lastOK = millis();
      }
      lastAT = millis();
    }
  } else if (diffOK > 5000) {
    if (diffAT > 500) {
      if (TestAT()) {
        lastOK = millis();
      }
      lastAT = millis();
    }
  }

  if (diffAT > 500 && (diffCREG > 15000 || (diffCREG > 5000 && !status_cache.service))) {
    //+CIND: 5,0,0,0,0,0,1
    //+CIND:("battchg",(0-5)), ("signal",(0-5)), ("service",(0,1)), ("message",(0,1)),("call",(0,1)), ("roam",(0,1)), ("smsfull",(0,1))
    status_cache = gsm.getStatus();
    lastCREG = millis();
  }

  if (!status_cache.service) {
    AT_STATUS = STATUS_UNREG;
  }
  if (status_cache.call) {
    AT_STATUS = STATUS_CALL;
  }

}

String Command(String cmd) {
  Serial.print("\n> " + cmd + "\n");
  FlushSerial2();
  Serial2.print(cmd + "\r");
  return ReadResponse();
}

SMSStruct smses[128];
SMSStruct *vsmses[128];
SMSStruct *current_sms;
int sms_count = 0;
int sms_pages_count = 0;
