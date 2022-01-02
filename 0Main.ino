void setup() {
  Serial.begin(115200);
  Serial2.begin(57600);

  Serial2.print("AT\r");
  //Serial2.readStringUntil("OK");
  Serial.setTimeout(1000);
  pixels.begin();
  SetLEDColor(0, 255, 0);

  pinMode(PIN_BTN_LED,OUTPUT);

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
      Serial2.print(buff+"\r");
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
  ExecuteQueue();
}

void CallBuffer() {
  String s = BufferToString(board_buffor, board_buffi);
  VoiceCall(&s);
  board_buffor[0] = 255;
  board_buffi = 0;
}

void InitInterrupt() {
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT), Board_Int, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_HANG), Hang_Int, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN), Button_Int, CHANGE);
}

long pressed = 0;
void Button_Int(){
  bool state = digitalRead(PIN_BTN);
  //Serial.print("S:");
  //Serial.println(state);
  if(state){
    pressed = millis();
  }else{
    long diff = millis()-pressed;
    if(diff > 600){
      ButtonAction(true);
    }else if(diff > 100){
      ButtonAction(false);
    }
  }
}
