I2CCom_Slave I2CComW(0xFF,&Wire);
#include "hardware/watchdog.h"

void WD_Init() {
  watchdog_enable(2000, 1);
  I2CComW.begin();
  I2CComW._wire->onReceive(WD_receiveEvent);
  I2CComW._wire->onRequest(WD_requestEvent);
  //Wire.setSDA(0);
  //Wire.setSCL(1);
  watchdog_update();
}

void WD_receiveEvent(int aid)
{
  I2CComW.receiveEvent();
  watchdog_update();
}

void WD_requestEvent()
{
  I2CComW.requestEvent();
}
