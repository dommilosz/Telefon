void ExecCommand(Stream *stream) {
  while (stream->available()) {
    char c = stream->read();
    if (c == '\r') {
      Serial2.print(buff + "\r");
      buff = "";
    }
    buff += c;
    if (c == '#') {
      char c = stream->read();
      if (c == 'd') {
        String s = "";
        s += (char)stream->read();
        Command_Keyboard(s);
      }
      if (c == 'l') {
        if (stream->read() == '1') {
          lcd.setLogger(true, &Serial);
        } else {
          lcd.setLogger(false, &Serial);
        }

      }
      if (c == 'g') {
        if (stream->read() == '1') {
          gsm.setLogger(true, &Serial);
        } else {
          gsm.setLogger(false, &Serial);
        }
      }
      if (c == 'b') {
        if (stream->read() == '1') {
          ButtonAction(true);
        } else {
          ButtonAction(false);
        }
      }
      if (c == 'h') {
        TakeATSemaphore();
        if (stream->read() == '1') {
          gsm.answer();
        } else {
          gsm.hangoff();
        }
        ReleaseATSemaphore();
      }
      buff = "";
    }
  }
}

void Command_Keyboard(String data) {
  BufferPush(data.toInt());
  lastInt = millis();
}
