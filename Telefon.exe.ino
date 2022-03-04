#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <GSMSim.h>
#include "coop_threads.h"
#include <ButtonDebounce.h>

#define THREAD_STACK_SIZE 0x500U

#define PIN_INPUT 2
#define InputRead() digitalRead(PIN_INPUT)

#define PIN_HANG  3
ButtonDebounce hng_button(PIN_HANG, 250);

#define PIN_RGB   4

#define STATUS_OK 0
#define STATUS_NOAT 1
#define STATUS_ERROR 2
#define STATUS_OTHER 3
#define STATUS_UNREG 4
#define STATUS_CALL 5

#include "I2CCom.h"
I2CCom_Master I2CCom(&Wire1);

const char *STATUS_STR[] = {"OK", "NOAT", "ERROR", "OTHER", "UNREG", "CALL"};
const char *AT_STATUSES[] = {"battchg", "signal", "service", "message", "call", "roam", "smsfull"};

long lastts = 0;
String b = "";

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
bool pe_error = false;

String LastMenuMsg = "";

Adafruit_NeoPixel pixels(1, PIN_RGB, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 20, 4, &Wire1);
GSMSim gsm(Serial2);

typedef void (* voidFunc)();
typedef void (* strFunc)(String r);
typedef void (* intFunc)(int r);
class MenuPanel;

struct SMSStruct2 {
  SMSStruct sms;
  int id;
};

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
const byte TEST_UI_MENU_ID = 17;
const byte MenuSMS_New_MENU_ID = 18;

String l_status = "ALL";
String buff = "";

struct MenuField {
  String txt;
  voidFunc action;
  bool valid, numbered;
};

class MenuPanel {
  public:
    String code;
    byte id;
    byte prev_id;
    MenuField fields[10];
    MenuField fields_sec[10];
    byte fields_count;
    byte draw_index;
    byte action_index;
    byte draw_index_div;
    bool double_time = false;
    bool append_field_index = true;
    intFunc generateCb = NULL;
    voidFunc draw_override = NULL;
    intFunc actionCb = NULL;

    void Draw() {
      lcd.setCursor(0, 1);
      if (draw_override != NULL) {
        draw_override();
        return;
      }

      if (menu != id)return;
      if (double_time) {
        draw_index = draw_index_div / 2;
      } else {
        draw_index = draw_index_div;
      }

      if (draw_index >= fields_count) {
        draw_index_div = 0;
        draw_index = 0;
      }

      if (generateCb != NULL)generateCb(draw_index);
      if (!fields[draw_index].valid) {
        draw_index_div++;
        Draw();
      }

      lcd.setCursor(0, 1);
      if (append_field_index || fields[draw_index].numbered) {
        lcd.print(draw_index);
        lcd.print(": ");
      }

      if (double_time && (fields_sec + draw_index) != NULL) {
        if ((draw_index_div % 2 == 1) && fields_sec[draw_index].valid) {
          lcd.print(fields_sec[draw_index].txt);
        } else {
          lcd.print(fields[draw_index].txt);
        }
      } else {
        lcd.print(fields[draw_index].txt);
      }

      draw_index_div++;
    }

    void Action(byte index) {
      action_index = index;
      if (actionCb != NULL)actionCb(index);
      if (index >= fields_count)return;
      if (!fields[index].valid)return;
      if (fields[index].action == NULL)return;

      fields[index].action();
    }

    void UpdateField(byte index, String txt, voidFunc action = NULL, bool numbered = false) {
      if (index >= fields_count)return;

      fields[index].txt = txt;
      fields[index].numbered = numbered;
      fields[index].action = action;
      fields[index].valid = true;
    }

    void UpdateField_D(byte index, String txt, bool numbered = false) {
      if (index >= fields_count)return;

      fields_sec[index].txt = txt;
      fields_sec[index].numbered = numbered;
      fields_sec[index].valid = true;
    }

    void UpdateField_Txt(byte index, String txt) {
      if (index >= fields_count)return;

      fields[index].txt = txt;
      fields[index].valid = true;
    }

    void SetSecoundaryField(byte index, String txt) {
      if (index >= fields_count)return;
      if (!fields[index].valid)return;

      fields_sec[index].txt = txt;
      fields_sec[index].valid = true;
    }

    void AddField(String txt, voidFunc action = NULL, bool numbered = false) {
      fields_count++;
      UpdateField(fields_count - 1, txt, action);
      fields[fields_count - 1].numbered = numbered;
    }

    void RemoveField(byte index) {
      for (byte i = index; i < 9; i++) {
        fields[i] = fields[i + 1];
        fields[i + 1].valid = false;
      }
      fields_count--;
    }

    void Back() {
      menu = prev_id;
    }

    void Show() {
      Reset();
      prev_id = menu;
      menu = id;
    }

    void SetGenerateCb(intFunc cb) {
      generateCb = cb;
    }

    void AddFields(byte c) {
      for (byte i = 0; i < c; i++) {
        AddField("");
      }
    }

    void AddExitField();

    void Reset() {
      draw_index = 0;
      draw_index_div = 0;
    }
};

MenuPanel menus[32];

MenuPanel *RegisterMenu(byte index, String code, bool append_field_index = false, bool double_time = false) {
  menus[index] = *(new MenuPanel());
  menus[index].code = code;
  menus[index].double_time = double_time;
  menus[index].id = index;
  menus[index].append_field_index = append_field_index;
  return &menus[index];
}
