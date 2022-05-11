void PrintBuffer(byte *buffer, int count) {
  for (int i = 0; i < count; i++) {
    Serial.print(buffer[i]);
  }
  Serial.println();
}

String BufferToString(byte *buffer, int count) {
  String s = "";
  for (int i = 0; i < count; i++) {
    s += buffer[i];
  }
  return s;
}

void SetLEDColor(uint8_t r, uint8_t g, uint8_t b) {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();   // Send the updated pixel colors to the hardware.
}

String forceStringToBeLength(String msg, int length) {
  int l = msg.length();
  if (l > length) {
    String s2 = msg.substring(l - length, l + length);
    Serial1.println(s2.length());
    return s2;
  }
  return msg;
}

uint8_t GetCoreNumber() {
  int a = 0;
  int _a = (int)&a;
  if (_a < (int)sp_core2) {
    return 1;
  }
  return 0;
}

void SetupCoreSP(uint8_t core) {
  int a = 0;
  if(core==0){
    sp_core1 = &a;
    return;
  }
  if(core==1){
    sp_core2 = &a;
    return;
  }
  throwError("Unknown core");
}
