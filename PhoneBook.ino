int pe_count;

void *_getPhoneBook() {
  pe_count = 0;

  for (int i = 0; i < 128; i++) {
    phoneBook[i].error = true;
  }

  TakeATSemaphore();
  gsm.getPhoneBookEntries(Cb_PhoneBook, 1, 99);
  ReleaseATSemaphore();


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
