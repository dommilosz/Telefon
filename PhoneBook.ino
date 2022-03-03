int pe_count;

void getPhoneBook() {
  pe_count = 0;

  for (int i = 0; i < 128; i++) {
    phoneBook[i].error = true;
  }

  TakeATSemaphore();
  gsm.getPhoneBookEntries(Cb_PhoneBook, 1, 128);
  ReleaseATSemaphore();

  pe_pages_count = ((pe_count - 1) / 7) + 1;
}

void Cb_PhoneBook(PhoneBookEntry pe) {
  phoneBook[pe_count] = pe;
  pe_count ++;
}
