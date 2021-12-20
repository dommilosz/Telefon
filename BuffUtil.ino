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

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
