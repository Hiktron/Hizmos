bool useNumbers = true;
bool useLetters = true;
bool useSymbols = true;
int passwordLength = 12;
int passwordToggleIndex = 0;
String generatedPassword = "";
bool passwordGenerated = false;
bool inGenMenu = false;
int genMenuIndex = 0; // 0 = back, 1 = QR

void handlePasswordMaker() {
  const char* options[] = { "Numbers", "Letters", "Symbols", "Length", "Generate" };
  int totalOptions = 5;

  // === Password View Menu ===
  if (passwordGenerated && inGenMenu) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Generated:");
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(0, 24, generatedPassword.c_str());

    // Draw "Back" button only
    u8g2.drawBox(40, 40, 48, 12);
    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(52, 49, "Back");
    u8g2.setDrawColor(1);

    if (!digitalRead(BTN_SELECT)) {
      delay(150);
      passwordGenerated = false;
      inGenMenu = false;
    }

    u8g2.sendBuffer();
    return;
  }

  // === Navigation ===
  if (!digitalRead(BTN_UP)) {
    passwordToggleIndex--;
    if (passwordToggleIndex < 0) passwordToggleIndex = totalOptions - 1;
    delay(150);
  }
  if (!digitalRead(BTN_DOWN)) {
    passwordToggleIndex++;
    if (passwordToggleIndex >= totalOptions) passwordToggleIndex = 0;
    delay(150);
  }

  // === Toggle or adjust values ===
  if (!digitalRead(BTN_LEFT) || !digitalRead(BTN_RIGHT)) {
    switch (passwordToggleIndex) {
      case 0: useNumbers = !useNumbers; break;
      case 1: useLetters = !useLetters; break;
      case 2: useSymbols = !useSymbols; break;
      case 3:
        if (!digitalRead(BTN_RIGHT) && passwordLength < 32) passwordLength++;
        if (!digitalRead(BTN_LEFT) && passwordLength > 4) passwordLength--;
        break;
    }
    delay(150);
  }

  // === Generate password ===
  if (!digitalRead(BTN_SELECT) && passwordToggleIndex == 4) {
    String charset = "";
    if (useNumbers) charset += "0123456789";
    if (useLetters) charset += "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (useSymbols) charset += "!@#$%^&*()_+=-<>?";
    generatedPassword = "";

    for (int i = 0; i < passwordLength; i++) {
      char c = charset[random(charset.length())];
      generatedPassword += c;
    }

    passwordGenerated = true;
    inGenMenu = true;
    delay(200);
    return;
  }

  // === Draw Main Menu ===
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, "Password Generator");

  for (int i = 0; i < totalOptions; i++) {
    int y = 20 + i * 10;
    if (i == passwordToggleIndex) {
      u8g2.drawBox(0, y - 8, 128, 10);
      u8g2.setDrawColor(0);
    }

    u8g2.drawStr(4, y, options[i]);

    if (i == 0) u8g2.drawStr(90, y, useNumbers ? "ON" : "OFF");
    if (i == 1) u8g2.drawStr(90, y, useLetters ? "ON" : "OFF");
    if (i == 2) u8g2.drawStr(90, y, useSymbols ? "ON" : "OFF");
    if (i == 3) {
      u8g2.drawFrame(85, y - 7, 38, 8);
      int barLen = map(passwordLength, 4, 32, 2, 36);
      u8g2.drawBox(86, y - 6, barLen, 6);
    }

    u8g2.setDrawColor(1);
  }

  u8g2.sendBuffer();
}


String generatePassword(int len, bool useNum, bool useLet, bool useSym) {
  String charset = "";
  if (useNum) charset += "0123456789";
  if (useLet) charset += "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  if (useSym) charset += "!@#$%^&*()_+-=<>?/[]{}";

  if (charset.length() == 0) return "ERR";

  String pass = "";
  for (int i = 0; i < len; i++) {
    pass += charset[random(charset.length())];
  }
  return pass;
}

