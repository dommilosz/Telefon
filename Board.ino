bool lastHS = false;

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

void Hang_Int() {
  bool hangState = HangRead();
  if (hangState) {
    gsm.hangoff();
  } else {
    gsm.answer();
  }
}

void FetchBoard() {
  long current = millis();

  if (board_count > 0 && current - lastInt > 500) {
    BufferPush(board_count);
    board_count = 0;
  }
  if ((millis() - lastInt > 5000 || menu != 0 || board_buffor[0] == 0) && board_buffi) {
    PrintBuffer(board_buffor, board_buffi);
    HandleBuffer();
  }
}

void BufferPush(byte b) {
  if (b == 10)b = 0;
  Serial.println(b);
  if (board_buffi < BOARD_MAX) {
    board_buffor[board_buffi] = b;
    board_buffi++;
  }
}
