void *_TestAT() {
  bool v = gsm.checkConnection(1);
  return (void *)v;
}

bool TestAT() {
  return (bool)DelegateTask(_TestAT);
}

GSMStatus status_cache;

String *__number;
void *_VoiceCall() {
  Serial.println(*__number);
  bool resp = gsm.call(__number);
  if (!resp) {
    LastMenuMsg = "ERR: " + resp;
  }
  return NULL;
}

void VoiceCall(String *number){
  __number = number;
  DelegateTask(_VoiceCall);
}

String op_name = "";
unsigned int pin_status = -1;
bool pin_enabled = false;

long lastOK   = -100000;
long lastCREG = -100000;

int CheckConnection() {
  AssertCore(0);
  long current = millis();
  long diffOK = current - lastOK;
  long diffCREG = current - lastCREG;

  if (!digitalRead(PIN_AT_STATUS)) {
    return STATUS_NOAT;
  }

  if (diffOK > 500) {
    gsm.setBaudrate(115200);
    if (TestAT()) {
      lastOK = millis();
    } else if (diffOK > 15000) {
      return STATUS_NOAT;
    }
  }

  if (diffCREG > 30000 || (diffCREG > 5000 && (!status_cache.service || pe_error))) {
    //+CIND: 5,0,0,0,0,0,1
    //+CIND:("battchg",(0-5)), ("signal",(0-5)), ("service",(0,1)), ("message",(0,1)),("call",(0,1)), ("roam",(0,1)), ("smsfull",(0,1))
    status_cache = gsm.getStatus();
    lastCREG = millis();
    op_name = String(gsm.operatorName());
    FetchPIN();
    gsm.setAudioChannel(1);
  }

  if (status_cache.service != 1) {
    return STATUS_UNREG;
  }
  if (status_cache.call == 1) {
    return STATUS_CALL;
  }
  if (pin_status != 0) {
    if (pin_status == 1) {
      LastMenuMsg = "ENTER PIN";
    }
    if (pin_status == 2) {
      LastMenuMsg = "ENTER PUK";
    }
  }
  return STATUS_OK;
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

void *_FetchUsage() {
  gsm.getPreferredSMSStorage(MemUsage);
  return NULL;
}

void FetchUsage(){
  DelegateTask(_FetchUsage);
}

void *_FetchPIN() {
  pin_status = gsm.pinStatus();
  _FetchPE();
  return NULL;
}

void FetchPIN() {
  DelegateTask(_FetchPIN);
}

void *_FetchPE() {
  int pe = gsm.getPinStatus();
  pe_error = false;
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
  return NULL;
}

void FetchPE() {
  DelegateTask(_FetchPE);
}
