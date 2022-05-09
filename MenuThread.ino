void setup1() {
  SetupCoreSP(1);
  
  board_buffi = 0;
  RegisterMenus();

  I2CCom.begin();
  I2CCom.OnDeviceConnected = OnConnected;
  I2CCom.OnDeviceDisconnected = OnDisconnected;
  I2CCom.OnData = OnInterrupt;
  I2CCom.Ignore(0x27);
  I2CCom.Ignore(0x28);

  pixels.begin();
  SetLEDColor(0, 255, 0);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  WD_Init();
  InitInterrupt();

  coop_sched_thread(refresh_screen, "refresh_lcd", THREAD_STACK_SIZE, NULL);
  coop_sched_thread(loop_5ms, "loop_5ms", THREAD_STACK_SIZE, NULL);
  coop_sched_thread(loop_1s, "loop_1s", THREAD_STACK_SIZE, NULL);
  coop_sched_thread(i2cScan, "i2c_scan", THREAD_STACK_SIZE, NULL);
  coop_sched_thread(i2cGcardRenderLoop, "i2c_gcard", THREAD_STACK_SIZE, NULL);
  coop_sched_service();
}


void loop1() {
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

void loop_5ms(void *arg) {
  while (true) {
    FetchBoard();
    hng_button.update();
    ExecCommand(&Serial);
    coop_idle(5);
  }
}

void i2cScan(void *arg) {
  while (true) {
    I2CCom.ScanDevices();
    WD_SendRefresh();
    coop_idle(50);
  }
}

void refresh_screen(void *arg) {
  while (true) {
    lcd.noCursor();
    DrawCurrentMenu();
    UpdateLED();
    int8_t data[1];
    data[0] = menu == MenuInput_MENU_ID;
    I2CCom.SendDataByID(20, 0x10, data, 1);
    coop_idle(500);
  }
}

bool _isLEDOn = false;
void loop_1s(void *arg) {
  while (true) {
    menuItem++;
    if (menuItem > 9) {
      menuItem = 0;
    }
    _isLEDOn = !_isLEDOn;
    digitalWrite(LED_BUILTIN, _isLEDOn);
    
    coop_idle(1000);
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

void InitInterrupt() {
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT), Board_Int, RISING);
  //attachInterrupt(digitalPinToInterrupt(PIN_HANG), Hang_Int, CHANGE);
  hng_button.setCallback(Hang_Int);
}
