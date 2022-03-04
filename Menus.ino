#define Menu_Show(id) {[]() {menus[id].Show();}}

void RegisterMenus() {
  RegisterMenu(0, "MAIN");

  MenuPanel *panel = RegisterMenu(MenuSelect_MENU_ID, "SEL", true, false);
  panel->AddExitField();
  panel->AddField("STATUS", Menu_Show(MenuStatus_MENU_ID));
  panel->AddField("TIMINGS", Menu_Show(MenuTimings_MENU_ID));
  panel->AddField("SMS", ShowMenuAndFetchSMS);
  panel->AddField("SETTINGS", Menu_Show(MenuSettings_MENU_ID));
  panel->AddField("PHONEBOOK", Menu_Show(MenuPhoneBook_Pre_MENU_ID));
  panel->AddField("TEST UI", Menu_Show(TEST_UI_MENU_ID));

  panel = RegisterMenu(MenuStatus_MENU_ID, "STA", false, false);
  panel->AddExitField();
  panel->AddFields(7);
  panel->SetGenerateCb(GenerateFields_Status);

  panel = RegisterMenu(MenuSMS_MENU_ID, "SMS", false, true);
  panel->AddExitField();
  panel->AddFields(9);
  panel->UpdateField(8, "Previous Page", SMS_PrevPage, true);
  panel->UpdateField(9, "Next Page", SMS_NextPage, true);
  panel->SetGenerateCb(GenerateFields_SMS);

  panel = RegisterMenu(MenuSMS_PRE_MENU_ID, "SMSP", true, false);
  panel->AddExitField();
  panel->AddFields(6);
  panel->AddField("New SMS", NewSMS_Show);
  panel->SetGenerateCb(GenerateFields_SMS_PRE);

  panel = RegisterMenu(MenuSMS_USAGE_MENU_ID, "USAG", false, false);
  panel->AddExitField();
  panel->AddFields(1);
  panel->SetGenerateCb(GenerateFields_SMS_USAGE);

  panel = RegisterMenu(MenuSMS_View_MENU_ID, "VIEW", true, false);
  panel->AddExitField();
  panel->AddFields(7);
  panel->UpdateField(5, "Text View", SMSViewAction_TextView, true);
  panel->UpdateField(6, "Mark as READ", SMSViewAction_Mark_Read, true);
  panel->UpdateField(7, "Delete", SMSViewAction_Delete, true);
  panel->SetGenerateCb(GenerateFields_SMS_View);

  panel = RegisterMenu(MenuSettings_MENU_ID, "SET", true, false);
  panel->AddExitField();
  panel->AddField("PIN", Menu_Show(MenuPIN_MENU_ID));

  panel = RegisterMenu(MenuPIN_MENU_ID, "PIN", true, false);
  panel->AddExitField();
  panel->AddFields(1);
  panel->AddField("Change PIN", PIN_Change_PIN);
  panel->SetGenerateCb(GenerateFields_PIN);

  panel = RegisterMenu(MenuPhoneBook_Pre_MENU_ID, "PHP", true, false);
  panel->AddExitField();
  panel->AddField("NEW [WIP]", NULL);
  panel->AddField("PHONEBOOK", PhoneBook_Launch);

  panel = RegisterMenu(MenuPhoneBook_MENU_ID, "PHB", true, true);
  panel->AddExitField();
  panel->AddFields(9);
  panel->UpdateField(8, "Previous Page", PE_PrevPage, true);
  panel->UpdateField(9, "Next Page", PE_NextPage, true);
  panel->SetGenerateCb(GenerateFields_PhoneBook);

  panel = RegisterMenu(MenuPhoneBook_View_MENU_ID, "VIEW", true, false);
  panel->AddExitField();
  panel->AddFields(4);
  panel->AddField("Call", PEViewAction_Call, true);
  panel->AddField("SMS [WIP]", SMS_Phonebook, true);
  panel->AddField("Edit", PEViewAction_Edit, true);
  panel->AddField("Delete", PEViewAction_Delete, true);
  panel->SetGenerateCb(GenerateFields_PE_View);

  panel = RegisterMenu(TEST_UI_MENU_ID, "TEUI", true, false);
  panel->AddExitField();
  panel->AddField("Normal Input", TestUI_NormalInput, true);
  panel->AddField("Text   Input", TestUI_TextInput, true);
  panel->AddField("Confirm Box", TestUI_Confirm, true);
  panel->AddField("TextView", TestUI_TextView, true);
  panel->AddField("TXDelay (3 ticks)", TestUI_TXD, true);

  panel = RegisterMenu(MenuSMS_New_MENU_ID, "NSMS", true, false);
  panel->AddExitField();
  panel->AddField("N: ", NewSMS_ChangeNumber);
  panel->AddField("T: ", NewSMS_ChangeText);
  panel->AddField("TextView", NewSMS_TextView);
  panel->AddField("SEND", NewSMS_Send);

  registerInputMenus();


  //const char *MENUS[] = {"", "", "", "", "", "", "", "TV", "", "", "", "INP", "CNF", "TXD", "", ""};
}

void Menu_Back() {
  menus[menu].Back();
}

inline void MenuPanel::AddExitField() {
  menus[menu].Back();
}
