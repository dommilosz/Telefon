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

int _hangState = 0;
void *_Hang_Int() {
  AssertCore(0);
  if (_hangState) {
    gsm.hangoff();
  } else {
    gsm.answer();
  }
  return NULL;
}

void Hang_Int(const int hangState) {
  _hangState = hangState;
  DelegateTask(_Hang_Int);
}

void FetchBoard() {
  long current = millis();

  if (board_count > 0 && current - lastInt > 500) {
    _BufferPush(board_count);
    board_count = 0;
  }
  if ((millis() - lastInt > 5000 || menu != 0 || board_buffor[0] == 0) && board_buffi) {
    PrintBuffer(board_buffor, board_buffi);
    HandleBuffer();
  }
}

void _BufferPush(byte b) {
  if (b == 10)b = 0;
  Serial.println(b);
  if (board_buffi < BOARD_MAX) {
    board_buffor[board_buffi] = b;
    board_buffi++;
  }
}

void BufferPush(byte b) {
  _BufferPush(b);
  FetchBoard();
}
