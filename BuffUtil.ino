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

char *BufferToCString(byte *buffer, int count) {
  return (char*)buffer;
}

void SetLEDColor(uint8_t r, uint8_t g, uint8_t b) {
  pixels.clear(); 
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void FlushSerial2() {
  while (Serial2.available()) {
    (Serial2.read());
  }
}

String ReadResponse() {
  Serial2.readStringUntil(13);
  Serial2.readStringUntil(10);

  String s = Serial2.readStringUntil(13);
  Serial2.readStringUntil(10);

  Serial.print(s);
  return s;
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
