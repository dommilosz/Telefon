String IO_str = "";
int input_prev_menu = 0;
long last_input = 0;
strFunc in_func;
int delayLeft = 0;
String _placeholder = "";

void ShowTextView(String txt){
  input_prev_menu = menu;
  menu = Menu_TV_MENU_ID;
}

void ShowInput(strFunc ifc,String placeholder=""){
  IO_str = "";
  input_prev_menu = menu;
  menu = MenuInput_MENU_ID;
  last_input = millis();
  in_func = ifc;
  _placeholder = placeholder;
}

void ShowConfirm(String txt,strFunc ifc){
  IO_str = txt;
  input_prev_menu = menu;
  menu = MenuConfirm_MENU_ID;
  in_func = ifc;
}

void ShowTXTD(String txt,int delay){
  IO_str = txt;
  input_prev_menu = menu;
  menu = Menu_TXTDelay_MENU_ID;
  delayLeft = delay;
}

//
//Menu_TV
//

void Menu_TV_Draw() {
  lcd.setCursor(0, 1);
  if (IO_str.length() > 16) {
    String msg = IO_str.substring(loopI500);
    if (msg.length() < 14) {
      loopI500 = 0;
    }
    lcd.print(msg);
  } else {
    lcd.print(IO_str);
  }


}

void Menu_TV_Action(byte item) {
  if (item == 0) {
    menu = input_prev_menu;
  }
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
  if((millis() - last_input)>15000){
    menu = input_prev_menu;
    in_func(IO_str);
  }
}

void MenuInput_Action(byte item) {
  if(IO_str.length()>16){
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

void Menu_Confirm_Action(byte item) {
  menu = input_prev_menu;
  in_func(String(item));
}

//
//Menu_Confirm
//

void Menu_TXTDelay_Draw() {
  lcd.setCursor(0, 1);
  lcd.print(IO_str);
  delayLeft --;
  if(delayLeft<=0){
    menu = input_prev_menu;
  }
}

void Menu_TXTDelay_Action(byte item) {
  
}
