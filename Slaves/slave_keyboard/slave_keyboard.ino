#include <Keypad.h>
#include "I2CCom.h"
I2CCom_Slave I2CCom(24, 127);

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
long startts;

void setup() {
  I2CCom.begin();
  I2CCom._wire->onReceive(receiveEvent);
  I2CCom._wire->onRequest(requestEvent);
  Serial.begin(57600);
  pinMode(LED_BUILTIN, OUTPUT);
  long startts = millis();
  while (!I2CCom.ready()) {
    //Serial.println(I2CCom.address);
    delay(10);
  }
  Serial.println(I2CCom.address);
  Serial.print("DHCP Took: ");
  Serial.println(millis() - startts);
}

void loop() {
  char customKey = customKeypad.getKey();
  digitalWrite(LED_BUILTIN, I2CCom.ready());

  if (customKey) {
    startts = millis();
    I2CCom.Interrupt(0x10, customKey);
  }
}

void receiveEvent()
{
  if (!I2CCom.receiveEvent()) {

  }
}

void requestEvent()
{
  uint8_t action = I2CCom.data_req;
  if (!I2CCom.requestEvent()) {

  }
}
