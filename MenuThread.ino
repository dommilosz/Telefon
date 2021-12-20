
long last5 = 0;
long last500 = 0;
long last1000 = 0;

int maxTimings[3]; 

void setup1() {
  last5 = millis();
  last500 = millis();
  last1000 = millis();
  InitInterrupt();
  board_buffi = 0;
}


void loop1() {
  long current = millis();

  if(current - last5>maxTimings[0])maxTimings[0] = current - last5;
  if(current - last500>maxTimings[1])maxTimings[1] = current - last500;
  if(current - last1000>maxTimings[2])maxTimings[2] = current - last1000;
  
  if ((current - last5) >= 5) {
    loop_5ms();
    loopI ++;
    if (loopI > 999) {
      loopI = 0;
    }
    last5 = millis();
  }
  if ((current - last500) >= 500) {
    loop_500ms();
    last500 = millis();
  }
  if ((current - last1000) >= 1000) {
    loop_1s();
    last1000 = millis();
  }
  delay(1);
}

void HandleBuffer() {
  if (menu == 0 && board_buffor[0] == 0 && board_buffi == 1) {
    menu = 1;
    board_buffi = 0;
    return;
  }

  if (menu == 1) {
    MenuSelect_Action(board_buffor[0]);
    board_buffi = 0;
    return;
  }
  if (menu == 2) {
    MenuStatus_Action(board_buffor[0]);
    board_buffi = 0;
    return;
  }
  if (menu == 3) {
    MenuTimings_Action(board_buffor[0]);
    board_buffi = 0;
    return;
  }
  if(menu == 4){
    MenuSMS_Action(board_buffor[0]);
    board_buffi = 0;
    return;
  }

  if(menu == 0){
    InvokeOnWorker(CallBuffer);
    return;
  }
}

void loop_5ms() {
  FetchBoard();
}

void loop_500ms() {
  DrawCurrentMenu();

  if ((AT_STATUS != _AT_STATUS) || (loopI % 5) == 0) {
    _AT_STATUS = AT_STATUS;
    UpdateLED();
  }
}

void loop_1s() {
  menuItem++;
  if (menuItem > 9) {
    menuItem = 0;
  }
}
