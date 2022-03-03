
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
  RegisterMenus();

  I2CCom.begin();
  I2CCom.OnDeviceConnected = OnConnected;
  I2CCom.OnDeviceDisconnected = OnDisconnected;
  I2CCom.OnData = OnInterrupt;
  I2CCom.Ignore(0x27);
}


void loop1() {
  long current = millis();

  if (current - last5 > maxTimings[0])maxTimings[0] = current - last5;
  if (current - last500 > maxTimings[1])maxTimings[1] = current - last500;
  if (current - last1000 > maxTimings[2])maxTimings[2] = current - last1000;

  if ((current - last1000) >= 1000) {
    loop_1s();
    last1000 = millis();
  } else if ((current - last500) >= 500) {
    loop_500ms();
    loopI500++;
    if (loopI500 > 999) {
      loopI500 = 0;
    }
    last500 = millis();
  } else if ((current - last5) >= 5) {
    loop_5ms();
    loopI ++;
    if (loopI > 999) {
      loopI = 0;
    }
    last5 = millis();
  }

  delay(1);
}

void HandleBuffer() {
  if (menu == 0 && board_buffor[0] == 0 && board_buffi == 1) {
    menu = 1;
  } else if (menu != 0) {
    menus[menu].Action(board_buffor[0]);
  } else if (menu == 0) {
    CallBuffer();
  }


  board_buffor[0] = 255;
  board_buffi = 0;
}

void loop_5ms() {
  FetchBoard();
  I2CCom.ScanDevices();
}

void loop_500ms() {
  lcd.noCursor();
  DrawCurrentMenu();

  if ((AT_STATUS != _AT_STATUS) || (loopI % 5) == 0) {
    _AT_STATUS = AT_STATUS;
    UpdateLED();
  }

  int8_t data[1];
  data[0] = menu == MenuInput_MENU_ID;
  I2CCom.SendDataByID(20, 0x10, data, 1);

  //I2CCom._wire->beginTransmission(8);
  //I2CCom._wire->write(0x10);
  //I2CCom._wire->write(menu == MenuInput_MENU_ID);
  //I2CCom._wire->endTransmission();
}

void loop_1s() {
  menuItem++;
  if (menuItem > 9) {
    menuItem = 0;
  }
}

void ButtonAction(bool isLong) {
  if (menu == MenuInput_MENU_ID) {
    if (isLong) {
      if (IO_str.length() < 1) {
        Menu_Back();
      } else {
        IO_str = IO_str.substring(0, IO_str.length() - 1);
      }
    } else {
      Menu_Back();
      in_func(IO_str);
    }
  }

  if (menu == 0) {
    if (board_buffi > 0) {
      board_buffi--;
    }
  }
}
