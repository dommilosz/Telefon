void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  Serial2.print("AT\r");
  //Serial2.readStringUntil("OK");
  Serial.setTimeout(1000);
  pixels.begin();
  SetLEDColor(0, 255, 0);

  lcd.init();                      // initialize the lcd
  lcd.backlight();

  board_buffor[0] = 255;
}

String buff = "";
void loop() {
  delay(5);
  AT_STATUS = STATUS_OK;
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r') {
      Command(buff);
      buff = "";
    }
    buff += c;
    if (c == '#') {
      if (Serial.read() == 'm') {
        board_buffi = 1;
        String s = "";
        s += (char)Serial.read();
        board_buffor[0] = s.toInt();
        HandleBuffer();
      }
      buff = "";
    }
  }

  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }

  CheckConnection();
  FetchHang();
  ExecuteQueue();
}

void CallBuffer() {
  String s = BufferToString(board_buffor, board_buffi);
  VoiceCall(&s);
  board_buffor[0] = 255;
  board_buffi = 0;
}
