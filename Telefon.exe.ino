#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <GSMSim.h>
#include "coop_threads.h"
#include <ButtonDebounce.h>
#include "PicoEasyReflash.h"

#define THREAD_STACK_SIZE 0x500U

#define PIN_INPUT 2
#define InputRead() digitalRead(PIN_INPUT)

#define PIN_HANG  3
ButtonDebounce hng_button(PIN_HANG, 250);

#define PIN_RGB   11
#define PIN_AT_STATUS   10

#define STATUS_OK 0
#define STATUS_NOAT 1
#define STATUS_ERROR 2
#define STATUS_OTHER 3
#define STATUS_UNREG 4
#define STATUS_CALL 5

#include "I2CCom.h"
I2CCom_Master I2CCom(&Wire1);
#include "GraphicsCardI2CCom.h"
GraphicsCardI2CCom gI2C(&I2CCom);

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
typedef void *(* ptrRetFunc)();

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
  String ftxt;
  voidFunc action;
  bool valid, numbered;
};

struct MainThreadTask {
  int status = 0;
  void *return_data;
  ptrRetFunc func;
};

byte task_mem_buff[128];
MainThreadTask tasks[16];
int task_index = 0;
void *DelegateTask(ptrRetFunc f) {
  while (task_index >= 16){
    coop_idle(100);
  }
  
  MainThreadTask *task = tasks + task_index;

  task->status = 0;
  task->func = f;
  task_index++;

  while (task->status != 0) {
    coop_idle(50);
  }
  return task->return_data;
}

void ExecuteTaskQueue() {
  if (task_index < 1)return;
  task_index--;
  MainThreadTask *task = tasks + task_index;
  task->return_data = task->func();
  task->status = 1;
}


bool gc_changed = false;
void SetAsset(int asset, String *txt) {
  int length = txt->length();
  char buff[length + 2];
  txt->toCharArray(buff, length + 1);
  buff[length + 1] = 0;
  if (gI2C.WriteAssetOrCRC(asset, (uint8_t*)buff, sizeof(buff)) != 250) {
    gc_changed = true;
  }
}

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
    String additional_info = ""; //page number, etc.
    String primary_line;
    String secondary_line;
    int secondary_trimType = 0;
    int primary_trimType = 0;
    int c1 = -1;
    int c2 = -1;

    void Draw() {
      c1 = -1;
      c2 = -1;
      additional_info = "";
      lcd.noCursor();
      lcd.setCursor(0, 1);
      secondary_trimType = 0;
      primary_trimType = 0;
      if (draw_override != NULL) {
        draw_override();
        lcd.setCursor(0, 0);
        if (primary_trimType == 1) {
          lcd.print(forceStringToBeLength(primary_line, 16));
        } else if (primary_trimType == 0) {
          lcd.print(primary_line.substring(0, 15));
        } else if (primary_trimType == 2) {
          String data = primary_line;
          if (primary_line.length() > 16) {
            String data = primary_line.substring(loopI500);
            if (data.length() < 14) {
              loopI500 = 0;
            }
          }
          if (data.length() > 16) {
            data = data.substring(0, 17);
          }
          lcd.print(data);
        }

        lcd.setCursor(0, 1);
        if (secondary_trimType == 1) {
          lcd.print(forceStringToBeLength(secondary_line, 16));
        } else if (secondary_trimType == 0) {
          lcd.print(secondary_line.substring(0, 15));
        } else if (secondary_trimType == 2) {
          String data = secondary_line;
          if (secondary_line.length() > 16) {
            data = secondary_line.substring(loopI500);
            if (data.length() < 14) {
              loopI500 = 0;
            }
          }
          if (data.length() > 16) {
            data = data.substring(0, 17);
          }
          lcd.print(data);
        }

        if (c1 >= 0 && c2 >= 0) {
          lcd.setCursor(c1, c2);
          lcd.cursor();
        }
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

      if (additional_info.length() > 0) {
        lcd.setCursor(0, 0);
        lcd.print("         ");
        lcd.setCursor(0, 0);
        lcd.print(additional_info);
        lcd.setCursor(0, 1);
      }


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
          lcd.print(fields_sec[draw_index].txt.substring(0, 15));
        } else {
          lcd.print(fields[draw_index].txt.substring(0, 15));
        }
      } else {
        lcd.print(fields[draw_index].txt.substring(0, 15));
      }

      draw_index_div++;
    }

    void DrawGC() {
      c1 = -1;
      c2 = -1;
      additional_info = "";
      if (draw_override != NULL) {
        draw_override();
        SetAsset(1, &primary_line);
        SetAsset(2, &secondary_line);

        SetAsset(3, &additional_info);
        SetAsset(4, &additional_info);
        SetAsset(5, &additional_info);
        SetAsset(6, &additional_info);
        SetAsset(7, &additional_info);
        SetAsset(8, &additional_info);
        SetAsset(9, &additional_info);
        SetAsset(10, &additional_info);

        if (c1 >= 0 && c2 >= 0) {
          //maybe do smth with cursor.
        }

        return;
      }

      if (menu != id)return;
      int draw_index = 0;
      for (int draw_index = 0; draw_index < 10; draw_index++) {
        if (generateCb != NULL)generateCb(draw_index);
        SetAsset(12, &additional_info);
        String asset = "";
        if (draw_index >= fields_count) {
          SetAsset(draw_index + 1, &asset);
          continue;
        }
        if (!fields[draw_index].valid) {
          continue;
        }

        if (append_field_index || fields[draw_index].numbered) {
          asset += draw_index;
          asset += ": ";
        }

        String field_txt = fields[draw_index].txt;
        if (fields[draw_index].ftxt.length() > 0) {
          field_txt = fields[draw_index].ftxt;
        }
        if (double_time && (fields_sec + draw_index) != NULL) {
          if (fields_sec[draw_index].valid) {
            asset += field_txt;
            asset += "   -  ";
            asset += fields_sec[draw_index].txt;
          } else {
            asset += field_txt;
          }
        } else {
          asset += field_txt;
        }
        SetAsset(draw_index + 1, &asset);

      }
    }

    void Action(byte index) {
      action_index = index;
      if (actionCb != NULL)actionCb(index);
      if (index >= fields_count)return;
      if (!fields[index].valid)return;
      if (fields[index].action == NULL)return;

      fields[index].action();
    }

    void setCursor(int x, int y) {
      c1 = x;
      c2 = y;
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

    void UpdateField_FTxt(byte index, String txt) {
      if (index >= fields_count)return;

      fields[index].ftxt = txt;
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
