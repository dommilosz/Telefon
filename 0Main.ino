void setup() {
  Serial.begin(115200);
  Serial2.begin(57600);

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

  ExecCommand(&Serial);

  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }

  CheckConnection();
  ExecuteQueue();
}

void CallBuffer() {
  String s = BufferToString(board_buffor, board_buffi);
  Serial.println(s);
  if (s.length() < 3)return;
  VoiceCall(&s);
}

void InitInterrupt() {
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT), Board_Int, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_HANG), Hang_Int, CHANGE);
}
