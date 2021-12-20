#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C_Wire1.h>

#define PIN_INPUT 2
#define InputRead() digitalRead(PIN_INPUT)

#define PIN_HANG  3
#define HangRead() digitalRead(PIN_HANG)

#define PIN_RGB   4

#define STATUS_OK 0
#define STATUS_NOAT 1
#define STATUS_ERROR 2
#define STATUS_OTHER 3
#define STATUS_UNREG 4
#define STATUS_BOARD 5
#define STATUS_CALL 6


const char *STATUS_STR[] = {"OK", "NOAT", "ERROR", "OTHER", "UNREG", "BOARD", "CALL"};
const char *AT_STATUSES[] = {"battchg", "signal", "service", "message", "call", "roam", "smsfull"};
const char *MENUS[] = {"MAIN", "SEL", "STA","TIME","SMS"};

long lastts = 0;
String b = "";
long lastOK = 0;
long lastAT = 0;
long lastCREG = 0;

uint8_t AT_STATUS = STATUS_OK;
uint8_t _AT_STATUS = STATUS_OK;
uint8_t last_reg_status = STATUS_UNREG;
int loopI = 0;
int menu = 0;
int menuItem = 0;

//BOARD Variables:
#define BOARD_MAX 9
byte board_buffor[BOARD_MAX];
int board_buffi = 0;
int board_count = 0;
long lastInt = 0;
byte sms_page = 0;

String LastMenuMsg = "";

Adafruit_NeoPixel pixels(1, PIN_RGB, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C_Wire1 lcd(0x27, 20, 4);

typedef void (* voidFunc)();

voidFunc InvokeQueue[16];
int invokePointer = 0;

void InvokeOnWorker(voidFunc func) {
  if (invokePointer < 15) {
    InvokeQueue[invokePointer] = func;
    invokePointer++;
  }
}

void ExecuteQueue() {
  if (invokePointer > 0) {
    invokePointer--;
    InvokeQueue[invokePointer]();
  }
}

struct SMSStruct{
  int id;
  String state,address,idk,date,data;
};
