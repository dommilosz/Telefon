void setup() {
  Serial.begin(115200);
  Serial2.begin(57600);
  Serial.setTimeout(1000);
  pixels.begin();
  SetLEDColor(0, 255, 0);

  lcd.init();
  lcd.backlight();

  InitInterrupt();

  board_buffor[0] = 255;
}

String buff = "";
void loop() {
  AT_STATUS = STATUS_OK;

  ExecCommand(&Serial);

  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }

  CheckConnection();
  hng_button.update();
}

void CallBuffer() {
  String s = BufferToString(board_buffor, board_buffi);
  Serial.println(s);
  if (s.length() < 3)return;
  VoiceCall(&s);
}

void InitInterrupt() {
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT), Board_Int, RISING);
  //attachInterrupt(digitalPinToInterrupt(PIN_HANG), Hang_Int, CHANGE);
  hng_button.setCallback(Hang_Int);
}
