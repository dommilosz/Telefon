String IO_str = "";
int input_prev_menu = 0;
long last_input = 0;
strFunc in_func;
int delayLeft = 0;
String _placeholder = "";

void ShowTextView(String txt) {
  menus[Menu_TV_MENU_ID].Show();
}

void ShowInput(strFunc ifc, String placeholder = "") {
  IO_str = "";
  menus[MenuInput_MENU_ID].Show();
  last_input = millis();
  in_func = ifc;
  _placeholder = placeholder;
}

void ShowConfirm(String txt, strFunc ifc) {
  IO_str = txt;
  menus[MenuConfirm_MENU_ID].Show();
  in_func = ifc;
}

void ShowTXTD(String txt, int delay) {
  IO_str = txt;
  input_prev_menu = menu;
  menus[Menu_TXTDelay_MENU_ID].Show();
  loopI500 = 0;
  delayLeft = delay;
}

//
//Menu_TV
//

void Menu_TV_Draw(int draw_index) {
  String data = IO_str;
  if (IO_str.length() > 16) {
    data = IO_str.substring(loopI500);
    if (data.length() < 14) {
      loopI500 = 0;
    }
  }
  menus[menu].UpdateField_Txt(0,data);
}

//
//Menu_Input
//

void MenuInput_Draw() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(_placeholder);
  lcd.setCursor(0, 1);
  lcd.print(IO_str);
  if ((millis() - last_input) > 15000) {
    menu = input_prev_menu;
    in_func(IO_str);
  }
}

void MenuInput_Action(int item) {
  if (IO_str.length() > 16) {
    return;
  }
  last_input = millis();
  IO_str += item;
}

//
//Menu_Confirm
//

void Menu_Confirm_Draw() {
  lcd.setCursor(0, 1);
  lcd.print(IO_str);
}

void Menu_Confirm_Action(int item) {
  menu = input_prev_menu;
  in_func(String(item));
}

void Menu_TXTDelay_Draw() {
  lcd.setCursor(0, 1);
  lcd.print(IO_str);
  delayLeft --;
  if (delayLeft <= 0) {
    menu = input_prev_menu;
  }
}

void registerInputMenus() {
  //const char *MENUS[] = {"", "", "", "", "", "", "", "TV", "", "", "", "INP", "CNF", "TXD", "", ""};
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
  panel->SetGenerateCb(Menu_TV_Draw);
}
