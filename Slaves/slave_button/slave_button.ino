#include "I2CCom.h"
#define PIN_BTN   3
#define PIN_BTN_LED 2

I2CCom_Slave I2CCom(20,&Wire1);

long startts;

void setup() {
  I2CCom.begin();
  I2CCom._wire->onReceive(receiveEvent);
  I2CCom._wire->onRequest(requestEvent);
  Serial.begin(57600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);
  long startts = millis();
  while (!I2CCom.ready()) {
    //Serial.println(I2CCom.address);
    delay(10);
  }
  Serial.println(I2CCom.address);
  Serial.print("DHCP Took: ");
  Serial.println(millis() - startts);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN), Button_Int, CHANGE);
}

void loop() {
  int ready = I2CCom.ready();
  digitalWrite(LED_BUILTIN, ready == 0);
  if (!(ready  == 0)) {
    //Serial.print("no ready: ");
    //Serial.println(ready);
    //Serial.print("A: ");
    //Serial.println(I2CCom.address);
  }
}

void receiveEvent(int aid)
{
  if (!I2CCom.receiveEvent()) {
    uint8_t action = I2CCom.data_req;
    if (action == 0x10) {
      if (I2CCom._wire->available())
        digitalWrite(PIN_BTN_LED, I2CCom._wire->read());
    }
  }

}

void requestEvent()
{
  if (!I2CCom.requestEvent()) {
    uint8_t action = I2CCom.data_req;

  }
}

long pressed = 0;
void Button_Int() {
  bool state = !digitalRead(PIN_BTN);
  if (state) {
    pressed = millis();
  } else {
    long diff = millis() - pressed;
    if (diff > 600) {
      I2CCom.Interrupt(0x10, 'B');
      Serial.println("B");
    } else if (diff > 100) {
      I2CCom.Interrupt(0x10, 'A');
      Serial.println("A");
    }
  }
}
