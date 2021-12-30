bool lastHS = false;

void InitInterrupt() {
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT), Board_Int, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_HANG), Hang_Int, CHANGE);
}

void Board_Int() {
  long current = millis();
  if (current - lastInt > 5000) {
    board_buffor[0] = 255;
    board_buffi = 0;
  }
  if (current - lastInt > 100) {
    lastInt = millis();
    board_count++;
  }
}

void Hang_Int(){
  bool hangState = HangRead();
  if(hangState){
    gsm.hangoff();
  }else{
    gsm.answer();
  }
}

void FetchBoard() {
  long current = millis();

  if (board_count > 0 && current - lastInt > 500) {
    if (board_count == 10)board_count = 0;
    Serial.println(board_count);
    if (board_buffi < BOARD_MAX) {
      board_buffor[board_buffi] = board_count;
      board_buffi++;
    }
    board_count = 0;

  }
  if ((millis() - lastInt > 5000 || menu != 0 || board_buffor[0] == 0) && board_buffi) {
    PrintBuffer(board_buffor, board_buffi);
    HandleBuffer();
  }

  if (AT_STATUS == STATUS_OK && board_count > 0) {
    AT_STATUS = STATUS_BOARD;
  }
}
