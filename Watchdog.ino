#include "hardware/watchdog.h"

void WD_Init() {
  watchdog_enable(1000, 1);
  Wire.setSDA(20);
  Wire.setSCL(21);
  Wire.begin(0x28);
  Wire.onReceive(WD_receiveEvent);
  watchdog_update();
}

void WD_SendRefresh() {
  Wire1.beginTransmission(0x28);
  Wire1.write(0xAA);
  Wire1.write(0x55);
  Wire1.endTransmission();
}

void WD_receiveEvent(int aid)
{
  if (Wire.available() < 2) {
    return;
  }
  byte b1 = Wire.read();
  byte b2 = Wire.read();
  if (b1 == 0xAA && b2 == 0x55) {
    watchdog_update();
  }

}
