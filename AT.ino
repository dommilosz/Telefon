bool TestAT() {
  return gsm.checkConnection(1);
}

GSMStatus status_cache;

void VoiceCall(String * number) {
  Serial.println(*number);
  bool resp = gsm.call(number);
  if (!resp) {
    LastMenuMsg = "ERR: " + resp;
  }
}

String op_name = "";
unsigned int pin_status = -1;
bool pin_enabled = false;

void CheckConnection() {
  long current = millis();
  long diffOK = current - lastOK;
  long diffAT = current - lastAT;
  long diffCREG = current - lastCREG;

  if (diffOK > 15000) {
    AT_STATUS = STATUS_NOAT;
    if (diffAT > 500) {
      //Serial2.begin(9600);
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

  if (AT_STATUS != STATUS_NOAT && diffAT > 500 && (diffCREG > 30000 || (diffCREG > 5000 && (!status_cache.service||pe_error)))) {
    //+CIND: 5,0,0,0,0,0,1
    //+CIND:("battchg",(0-5)), ("signal",(0-5)), ("service",(0,1)), ("message",(0,1)),("call",(0,1)), ("roam",(0,1)), ("smsfull",(0,1))
    status_cache = gsm.getStatus();
    lastCREG = millis();
    op_name = String(gsm.operatorNameFromSim());
    FetchPIN();
  }

  if (status_cache.service != 1) {
    AT_STATUS = STATUS_UNREG;
  }
  if (status_cache.call == 1) {
    AT_STATUS = STATUS_CALL;
  }
  if (pin_status != 0) {
    if (pin_status == 1) {
      LastMenuMsg = "ENTER PIN";
    }
    if (pin_status == 2) {
      LastMenuMsg = "ENTER PUK";
    }
  }

}

SMSStruct smses[128];
PhoneBookEntry phoneBook[128];

SMSStruct *vsmses[128];
SMSStruct *current_sms;
PhoneBookEntry *current_pe;
int sms_count = 0;
int sms_pages_count = 0;
int pe_pages_count = 0;

int MemUsage[] = {0, 0};

void FetchUsage() {
  gsm.getPreferredSMSStorage(MemUsage);
}

void FetchPIN() {
  pin_status = gsm.pinStatus();
  FetchPE();
}

void FetchPE() {
  int pe = gsm.getPinStatus();
  pe_error= false;
  if (pe == 0) {
    pin_enabled = false;
  }
  else if (pe == 1) {
    pin_enabled = true;
  } else {
    Serial.println("E57");
    Serial.println(pe);
    pe_error = true;
  }
}
