String IO_str = "";
long last_input = 0;
byte last_input_str = -1;
byte str_iteration = 0;
strFunc in_func;
int delayLeft = 0;
String _placeholder = "";
bool _is_string = false;
bool _disableLimit = false;

void ShowTextView(String txt) {
  IO_str = txt;
  menus[Menu_TV_MENU_ID].Show();
}

void ShowInput(strFunc ifc, String placeholder = "", bool isString = false, bool disableLimit = false) {
  IO_str = "";
  menus[MenuInput_MENU_ID].Show();
  last_input = millis();
  in_func = ifc;
  _placeholder = placeholder;
  _is_string = isString;
  _disableLimit = disableLimit;
}

void ShowConfirm(String txt, strFunc ifc) {
  IO_str = txt;
  menus[MenuConfirm_MENU_ID].Show();
  in_func = ifc;
}

void ShowTXTD(String txt, int delay) {
  IO_str = txt;
  menus[Menu_TXTDelay_MENU_ID].Show();
  loopI500 = 0;
  delayLeft = delay;
}

//
//Menu_TV
//

void Menu_TV_Draw() {
  String data = IO_str;
  menus[menu].secondary_trimType = 2;
  menus[menu].primary_line = "TextView:";
  menus[menu].secondary_line = data;
}

//
//Menu_Input
//

void MenuInput_Draw() {
  menus[menu].primary_line = _placeholder;
  menus[menu].secondary_line = IO_str;
  menus[menu].secondary_trimType = 1;
  if ((millis() - last_input) > 15000) {
    Menu_Back();
    in_func(IO_str);
  }

  if ((millis() - last_input) < 5000 || !_is_string) {
    menus[menu].setCursor(IO_str.length() - 1, 1);
  } else {
    menus[menu].setCursor(IO_str.length(), 1);
  }

}


// -------------------------
// |   1   |   2   |   3   |
// |  .,?! |  abc  |  def  |
// -------------------------
// |   4   |   5   |   6   |
// |  ghi  |  jkl  |  mno  |
// -------------------------
// |   7   |   8   |   9   |
// |  pqrs |  tuv  |  wxyz |
// -------------------------
// |       |   0   |       |
// |       | space |       |
// -------------------------

String str_characters[] = {"  ", ".,?!", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

void MenuInput_Action(int item) {
  if (IO_str.length() > 16 && !_disableLimit) {
    return;
  }
  if (_is_string) {
    if (item == last_input_str && ((millis() - last_input) < 5000)) {
      str_iteration++;
      if (str_iteration >= str_characters[last_input_str].length()) {
        str_iteration = 0;
      }
      Serial.println(str_characters[last_input_str]);
      Serial.println(IO_str.length());
      Serial.println(str_characters[last_input_str][str_iteration]);
      IO_str[IO_str.length() - 1] = str_characters[last_input_str].substring(str_iteration, str_iteration + 1)[0];
    } else {
      str_iteration = 0;
      last_input_str = item;
      IO_str += str_characters[last_input_str].substring(0, 1);
    }
    last_input_str = item;
  } else {
    IO_str += item;
  }

  last_input = millis();
}

//
//Menu_Confirm
//

void Menu_Confirm_Draw() {
  menus[menu].secondary_line = IO_str;
}

void Menu_Confirm_Action(int item) {
  Menu_Back();
  in_func(String(item));
}

void Menu_TXTDelay_Draw() {
  menus[menu].secondary_line = IO_str;
  delayLeft --;
  if (delayLeft <= 0) {
    Menu_Back();
  }
}

void registerInputMenus() {
  MenuPanel *panel = RegisterMenu(MenuInput_MENU_ID, "INP", false, false);
  panel->draw_override = MenuInput_Draw;
  panel->actionCb = MenuInput_Action;

  panel = RegisterMenu(Menu_TXTDelay_MENU_ID, "TXD", false, false);
  panel->draw_override = Menu_TXTDelay_Draw;

  panel = RegisterMenu(MenuConfirm_MENU_ID, "CNF", false, false);
  panel->draw_override = Menu_Confirm_Draw;
  panel->actionCb = Menu_Confirm_Action;

  panel = RegisterMenu(Menu_TV_MENU_ID, "TV", false, false);
  panel->AddField("", Menu_Back);
  panel->draw_override = Menu_TV_Draw;
}

//
//UI Test
//

void TestUI_NormalInput() {
  ShowInput(TestUI_Result_Function, "Test placeholder", false);
}

void TestUI_TextInput() {
  ShowInput(TestUI_Result_Function, "Test placeholder", true);
}

void TestUI_Confirm() {
  ShowConfirm("Test confirm box", TestUI_Result_Function);
}

void TestUI_TextView() {
  ShowTextView("Test TextView: very long text that doesn't seem to fit into this small i2c lcd screen");
}

void TestUI_TXD() {
  ShowTXTD("3 ticks later...", 3);
}

void TestUI_Result_Function(String txt) {
  String data = "R: ";
  data += txt;
  ShowTXTD(data, 2);
}
