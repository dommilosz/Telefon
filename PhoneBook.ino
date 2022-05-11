int pe_count;

void *_getPhoneBook() {
  AssertCore(0);
  pe_count = 0;

  for (int i = 0; i < 128; i++) {
    phoneBook[i].error = true;
  }
  gsm.getPhoneBookEntries(Cb_PhoneBook, 1, 99);
  pe_pages_count = ((pe_count - 1) / 7) + 1;
  return NULL;
}

void getPhoneBook() {
  DelegateTask(_getPhoneBook);
}

void Cb_PhoneBook(PhoneBookEntry pe) {
  phoneBook[pe_count] = pe;
  pe_count ++;
}

String phb_number;
String phb_name;
int phb_id;

void NewPHB_Show() {
  menus[NewPhonebook_MENU_ID].Show();
  NewPHB_ChangeNumber("");
  NewPHB_ChangeName("");
}

void NewPHB_ShowEdit(PhoneBookEntry *pe) {
  menus[NewPhonebook_MENU_ID].Show();
  NewPHB_ChangeNumber(pe->phoneno);
  NewPHB_ChangeName(pe->name);
  NewPHB_ChangeID(pe->id);
}

void NewPHB_ChangeNumber() {
  ShowInput(NewPHB_ChangeNumber, "Enter NUMBER: ");
}

void NewPHB_ChangeNumber(String number) {
  phb_number = number;
  String d = "Numb: ";
  d += number;
  menus[menu].UpdateField_Txt(1, d);
}

void NewPHB_ChangeName() {
  ShowInput(NewPHB_ChangeName, "Enter Name: ", true, true);
}

void NewPHB_ChangeName(String text) {
  phb_name = text;
  String d = "Name: ";
  d += text;
  menus[menu].UpdateField_Txt(2, d);
}

void NewPHB_ChangeID(int id){
  phb_id = id;
  String d = "ID: ";
  d += phb_id;
  menus[menu].UpdateField_Txt(3, d);
}

void NewPHB_ChangeID(String id){
  NewPHB_ChangeID(id.toInt());
}

void NewPHB_ChangeID(){
  ShowInput(NewPHB_ChangeID, "Enter ID: ", false, false);
}

void NewPHB_Add() {
  DelegateTask(_NewPHB_Add);
}

void *_NewPHB_Add() {
  AssertCore(0);
  PhoneBookEntry pe;
  pe.id = phb_id;
  pe.name = phb_name;
  pe.phoneno = phb_number;
  pe.type = 129;
  if (phb_number.length() > 2 && phb_name.length() > 0) {
    if (gsm.setPhoneBookEntry(&pe)) {
      Menu_Back();
      ShowTXTD("Saved!", 4);
    } else {
      ShowTXTD("ERROR!", 4);
    }
  } else {
    if (phb_number.length() <= 2) {
      ShowTXTD("SHORT NUMBER!", 4);
    } else {
      ShowTXTD("Name NEEDED!", 4);
    }
  }
  return NULL;
}
