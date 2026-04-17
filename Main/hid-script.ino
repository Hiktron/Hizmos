
// ========== HIDScript: File Variables ==========
String hidFileList[20];
int hidFileCount = 0;
int hidSelectedIndex = 0;
unsigned long hidDefaultDelay = 300;
String hidLastCommand = "";

// ========== Debug Output ==========
void hidShowDebug(const String& msg) {
  Serial.println(msg);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(0, 24, msg.c_str());
  u8g2.sendBuffer();
}

// ========== Key Mapper ==========
uint8_t hidGetSpecialKey(String key) {
  key.toUpperCase();
  if (key == "ENTER") return KEY_RETURN;
  if (key == "TAB") return KEY_TAB;
  if (key == "ESC" || key == "ESCAPE") return KEY_ESC;
  if (key == "SPACE") return ' ';
  if (key == "UPARROW") return KEY_UP_ARROW;
  if (key == "DOWNARROW") return KEY_DOWN_ARROW;
  if (key == "LEFTARROW") return KEY_LEFT_ARROW;
  if (key == "RIGHTARROW") return KEY_RIGHT_ARROW;
  if (key == "DELETE") return KEY_DELETE;
  if (key == "BACKSPACE") return KEY_BACKSPACE;
  if (key == "INSERT") return KEY_INSERT;
  if (key == "HOME") return KEY_HOME;
  if (key == "END") return KEY_END;
  if (key == "PAGEUP") return KEY_PAGE_UP;
  if (key == "PAGEDOWN") return KEY_PAGE_DOWN;
  return 0;
}

// ========== Combo Key Press ==========
void hidPressCombo(bool gui, bool alt, bool ctrl, bool shift, uint8_t key) {
  if (gui) Keyboard.press(KEY_LEFT_GUI);
  if (alt) Keyboard.press(KEY_LEFT_ALT);
  if (ctrl) Keyboard.press(KEY_LEFT_CTRL);
  if (shift) Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(key);
  delay(100);
  Keyboard.releaseAll();
}

// ========== Execute HID Script ==========
void hidRunScript(const char* path) {
  File file = SD.open(path);
  if (!file) {
    hidShowDebug("FAILED TO OPEN FILE");
    return;
  }

  hidShowDebug(String("Running: ") + path);

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) continue;

    Serial.println("CMD: " + line);
    hidShowDebug("CMD: " + line);
    String upperLine = line;
    upperLine.toUpperCase();

    if (upperLine.startsWith("REM") || upperLine.startsWith("COMMENT")) continue;
    if (upperLine.startsWith("DEFAULT_DELAY ")) {
      hidDefaultDelay = upperLine.substring(14).toInt();
      continue;
    }
    if (upperLine.startsWith("STRING ")) {
      Keyboard.print(line.substring(7));
      hidLastCommand = line;
    } else if (upperLine.startsWith("DELAY ")) {
      delay(upperLine.substring(6).toInt());
    } else if (upperLine.startsWith("REPEAT ")) {
      int repeatCount = upperLine.substring(7).toInt();
      for (int i = 0; i < repeatCount; i++) {
        hidRunScript(path);
      }
      break;
    } else if (upperLine.startsWith("PRINT ")) {
      hidShowDebug(line.substring(6));
    } else if (upperLine.startsWith("TYPE ")) {
      char key = line.substring(5).charAt(0);
      Keyboard.press(key);
      delay(100);
      Keyboard.release(key);
    } else if (hidGetSpecialKey(upperLine) > 0) {
      uint8_t key = hidGetSpecialKey(upperLine);
      Keyboard.press(key);
      delay(100);
      Keyboard.release(key);
    } else if (upperLine.startsWith("GUI ") || upperLine.startsWith("WINDOWS ")) {
      uint8_t key = line.substring(line.indexOf(' ') + 1).charAt(0);
      hidPressCombo(true, false, false, false, key);
    } else if (upperLine.startsWith("CTRL ") || upperLine.startsWith("CONTROL ")) {
      uint8_t key = line.substring(line.indexOf(' ') + 1).charAt(0);
      hidPressCombo(false, false, true, false, key);
    } else if (upperLine.startsWith("ALT ")) {
      uint8_t key = line.substring(4).charAt(0);
      hidPressCombo(false, true, false, false, key);
    } else if (upperLine.startsWith("SHIFT ")) {
      uint8_t key = line.substring(6).charAt(0);
      hidPressCombo(false, false, false, true, key);
    }

    delay(hidDefaultDelay);
  }

  file.close();
  hidShowDebug("Done.");
  delay(1000);
}

// ========== Draw File Menu ==========
void hidDrawMenu() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);
  for (int i = 0; i < hidFileCount && i < 5; i++) {
    int idx = (hidSelectedIndex / 5) * 5 + i;
    if (idx >= hidFileCount) break;

    if (idx == hidSelectedIndex) {
      u8g2.drawBox(0, i * 12, 128, 12);
      u8g2.setDrawColor(0);
    } else {
      u8g2.setDrawColor(1);
    }

    u8g2.setCursor(2, (i + 1) * 12 - 2);
    u8g2.print(hidFileList[idx]);
    u8g2.setDrawColor(1);
  }
  u8g2.sendBuffer();
}

// ========== HIDScript Initialization ==========
void hidInit() {



// Start SD card with HSPI
if (!SD.begin(SD_CS, SD_SPI)) {
  runLoop(drawnosdcard);
}

  
  File root = SD.open("/");
  while (true) {
    File entry = root.openNextFile();
    if (!entry) break;

    String name = entry.name();
    if (!entry.isDirectory() && name.endsWith(".duck") && name.indexOf(' ') == -1) {
      hidFileList[hidFileCount++] = name;
    }
    entry.close();
  }

  hidShowDebug("Ready.");
}



void hidscriptmenu() {
  hidDrawMenu();

  if (digitalRead(BTN_UP) == LOW) {
    hidSelectedIndex = (hidSelectedIndex - 1 + hidFileCount) % hidFileCount;
    delay(200);
  } else if (digitalRead(BTN_DOWN) == LOW) {
    hidSelectedIndex = (hidSelectedIndex + 1) % hidFileCount;
    delay(200);
  } else if (digitalRead(BTN_SELECT) == LOW) {
    String path = "/" + hidFileList[hidSelectedIndex];
    hidRunScript(path.c_str());
    delay(500);
  }
}








