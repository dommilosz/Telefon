void OnConnected(int address) {
  Serial.print("Connected: ");
  Serial.println(address);
}

void OnDisconnected(int address) {
  Serial.print("Disconnected: ");
  Serial.println(address);
}

void OnInterrupt(int addr, int type, int length) {
  Serial.print("#######");
  Serial.print("INT: ");
  Serial.print(addr);
  Serial.print(" - ");
  Serial.println(type);

  I2CCom.RequestData(addr, type, length);

  if (type == 0x01) {
    ExecCommand(&Wire1);
  }
  if (type == 0x10) {
    char c = I2CCom._wire->read();
    Serial.println(c);
    if (c >= '0' && c <= '9') {
      Command_Keyboard(String(c));
    } else if (c == 'A') {
      //short press...
      ButtonAction(false);
    } else if (c == 'B') {
      //long press...
      ButtonAction(true);
    } else if (c == 'C') {
      Hang_Int(0); //Answer
    } else if (c == 'D') {
      Hang_Int(1); //Hang
    }

  }
}
