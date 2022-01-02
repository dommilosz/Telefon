#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <GSMSim.h>

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


long lastts = 0;
String b = "";
long lastOK = -150000;
long lastAT = 0;
long lastCREG = 0;

uint8_t AT_STATUS = STATUS_OK;
uint8_t _AT_STATUS = STATUS_OK;
int loopI = 0;
int loopI500 = 0;
int menu = 0;
int menuItem = 0;

//BOARD Variables:
#define BOARD_MAX 9
byte board_buffor[BOARD_MAX];
int board_buffi = 0;
int board_count = 0;
long lastInt = 0;
byte sms_page = 0;
int sms_menu_item = 0;
int selected_sms = -1;

String LastMenuMsg = "";

Adafruit_NeoPixel pixels(1, PIN_RGB, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 20, 4, &Wire1);
GSMSim gsm(Serial2);

typedef void (* voidFunc)();
typedef void (* strFunc)(String r);

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
    
    InvokeQueue[invokePointer-1]();
    invokePointer--;
  }
}

struct SMSStruct2{
  SMSStruct sms;
  int id;
};

const char *MenuStdExit_MENU[1] = {"EXIT"};
const char *MENUS[] = {"MAIN", "SEL", "STA","TIME","SMS","SMSP","VIEW","TV","USAG","SET","PIN","INP","CNF","TXD","PHP","PHB"};

const byte MenuSelect_MENU_LEN = 6;
const char *MenuSelect_MENU[MenuSelect_MENU_LEN] = {"EXIT", "STATUS", "TIMINGS", "SMS","SETTINGS","PHONEBOOK"};

const byte MenuSelect_MENU_ID = 1;
const byte MenuStatus_MENU_ID = 2;
const byte MenuTimings_MENU_ID = 3;
const byte MenuSMS_MENU_ID = 4;
const byte MenuSMS_PRE_MENU_ID = 5;
const byte MenuSMS_View_MENU_ID = 6;
const byte Menu_TV_MENU_ID = 7;
const byte MenuSMS_USAGE_MENU_ID = 8;
const byte MenuSettings_MENU_ID = 9;
const byte MenuPIN_MENU_ID = 10;
const byte MenuInput_MENU_ID = 11;
const byte MenuConfirm_MENU_ID = 12;
const byte Menu_TXTDelay_MENU_ID = 13;
const byte MenuPhoneBook_Pre_MENU_ID = 14;
const byte MenuPhoneBook_MENU_ID = 15;
const byte MenuPhoneBook_View_MENU_ID = 16;

String l_status = "ALL";
