bool TestAT() {
  TakeATSemaphore();
  bool v = gsm.checkConnection(1);
  ReleaseATSemaphore();
  return v;
}

GSMStatus status_cache;

void VoiceCall(String * number) {
  Serial.println(*number);
  TakeATSemaphore();
  bool resp = gsm.call(number);
  ReleaseATSemaphore();
  if (!resp) {
    LastMenuMsg = "ERR: " + resp;
  }
}

String op_name = "";
unsigned int pin_status = -1;
bool pin_enabled = false;

long lastOK   = -100000;
long lastCREG = -100000;

int CheckConnection() {
  long current = millis();
  long diffOK = current - lastOK;
  long diffCREG = current - lastCREG;

  if (diffOK > 500) {
    if (TestAT()) {
      lastOK = millis();
    } else if (diffOK > 15000) {
      return STATUS_NOAT;
    }
  }

  if (diffCREG > 30000 || (diffCREG > 5000 && (!status_cache.service || pe_error))) {
    //+CIND: 5,0,0,0,0,0,1
    //+CIND:("battchg",(0-5)), ("signal",(0-5)), ("service",(0,1)), ("message",(0,1)),("call",(0,1)), ("roam",(0,1)), ("smsfull",(0,1))
    TakeATSemaphore();
    status_cache = gsm.getStatus();
    ReleaseATSemaphore();
    lastCREG = millis();
    TakeATSemaphore();
    op_name = String(gsm.operatorNameFromSim());
    ReleaseATSemaphore();
    FetchPIN();
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

void FetchUsage() {
  TakeATSemaphore();
  gsm.getPreferredSMSStorage(MemUsage);
  ReleaseATSemaphore();
}

void FetchPIN() {
  TakeATSemaphore();
  pin_status = gsm.pinStatus();
  ReleaseATSemaphore();
  FetchPE();
}

void FetchPE() {
  TakeATSemaphore();
  int pe = gsm.getPinStatus();
  ReleaseATSemaphore();
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
}

bool at_semaphore = false;
void TakeATSemaphore() {
  while (at_semaphore) {

  }
  at_semaphore = true;
}

void ReleaseATSemaphore() {
  at_semaphore = false;
}
