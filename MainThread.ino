void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.setTimeout(1000);
  lcd.init();
  lcd.backlight();

  board_buffor[0] = 255;
}

int ring_find_status = 0;
void loop() {
  ExecCommand(&Serial);

  if (!at_semaphore && Serial2.available()) {
    while (Serial2.available()) {
      char c = Serial2.read();

      if (c == 'R')ring_find_status = 1;
      else if (c == 'I' && ring_find_status == 1)ring_find_status = 2;
      else if (c == 'N' && ring_find_status == 2)ring_find_status = 3;
      else if (c == 'G' && ring_find_status == 3)ring_find_status = 4;
      else ring_find_status = 0;

      if (ring_find_status == 4) {
        //SetLEDColor(255, 255, 255);
      }

      Serial.write(c);
    }
  }

  //TickEasyReflash();
  AT_STATUS = CheckConnection();
  ExecuteTaskQueue();
  delay(5);
}

void CallBuffer() {
  String s = BufferToString(board_buffor, board_buffi);
  Serial.println(s);
  if (s.length() < 3)return;
  VoiceCall(&s);
}
