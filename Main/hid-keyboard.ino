
#define FONT u8g2_font_6x12_tf

const char* hidKeyboard[] = {
  "abcdefghi",
  "jklmnopqr",
  "stuvwxyz ",
  "123456789",
  "0!@#$%^&*",
  "()_+-=<>?",
  "/\\[]{}|~",
  ";:'\",.` ",
};

const int hidRows = sizeof(hidKeyboard) / sizeof(hidKeyboard[0]);
const int hidCols = 9;

int hidSelectedRow = 0;
int hidSelectedCol = 0;
int hidScrollOffset = 0;

const int hidVisibleRows = 4;
const int hidBoxW = 13;
const int hidBoxH = 14;

struct HIDSpecialKey {
  const char* label;
  int width;
  uint8_t keyCode;
};

HIDSpecialKey hidSpecialKeys[] = {
  {"Back", 28, KEY_BACKSPACE},
  {"Enter", 32, KEY_RETURN},
  {"Tab", 24, KEY_TAB},
  {"Esc", 24, KEY_ESC},
  {"Space", 38, ' '},
  {"Up", 20, KEY_UP_ARROW},
  {"Down", 30, KEY_DOWN_ARROW},
  {"Left", 26, KEY_LEFT_ARROW},
  {"Right", 30, KEY_RIGHT_ARROW},
  {"Shift", 30, KEY_LEFT_SHIFT},
  {"Ctrl", 26, KEY_LEFT_CTRL},
  {"Alt", 20, KEY_LEFT_ALT},
  {"Del", 24, KEY_DELETE}
};

int hidSelectedSpecial = 0;
bool hidInSpecialKeys = false;
const int hidSpecialKeysCount = sizeof(hidSpecialKeys) / sizeof(hidSpecialKeys[0]);
const int hidSpecialKeysPerRow = 4;
const int hidSpecialRowHeight = hidBoxH + 2;
int hidBaseY = hidVisibleRows * hidBoxH + 2;

bool isPressed(int pin) {
  return digitalRead(pin) == LOW;
}

void drawHIDKeyboard() {
  u8g2.clearBuffer();
  u8g2.setFont(FONT);

  // Draw main keyboard
  if (!hidInSpecialKeys) {
    for (int row = hidScrollOffset; row < hidScrollOffset + hidVisibleRows && row < hidRows; row++) {
      for (int col = 0; col < hidCols; col++) {
        int x = col * hidBoxW;
        int y = (row - hidScrollOffset) * hidBoxH;
        char ch = hidKeyboard[row][col];

        if (row == hidSelectedRow && col == hidSelectedCol) {
          u8g2.drawBox(x, y, hidBoxW, hidBoxH);
          u8g2.setDrawColor(0);
          u8g2.drawGlyph(x + 3, y + 11, ch);
          u8g2.setDrawColor(1);
        } else {
          u8g2.drawFrame(x, y, hidBoxW, hidBoxH);
          u8g2.drawGlyph(x + 3, y + 11, ch);
        }
      }
    }

    // Scroll bar
    if (hidRows > hidVisibleRows) {
      int barHeight = (hidVisibleRows * hidBoxH) / hidRows;
      int barY = (hidScrollOffset * hidBoxH * hidVisibleRows) / hidRows;
      u8g2.drawFrame(127, 0, 1, hidVisibleRows * hidBoxH);
      u8g2.drawBox(127, barY, 1, barHeight);
    }
  }

  // Draw special keys
  if (hidInSpecialKeys) {
    int xPos = 0;
    int yPos = hidBaseY - (hidVisibleRows * hidBoxH);

    for (int i = 0; i < hidSpecialKeysCount; i++) {
      if (i > 0 && i % hidSpecialKeysPerRow == 0) {
        xPos = 0;
        yPos += hidSpecialRowHeight;
      }

      bool isSelected = (i == hidSelectedSpecial);

      if (isSelected) {
        u8g2.drawBox(xPos, yPos, hidSpecialKeys[i].width, hidBoxH);
        u8g2.setDrawColor(0);
      } else {
        u8g2.drawFrame(xPos, yPos, hidSpecialKeys[i].width, hidBoxH);
      }

      int textWidth = u8g2.getStrWidth(hidSpecialKeys[i].label);
      int textX = xPos + (hidSpecialKeys[i].width - textWidth) / 2;
      u8g2.setCursor(textX, yPos + 11);
      u8g2.print(hidSpecialKeys[i].label);

      if (isSelected) {
        u8g2.setDrawColor(1);
      }

      xPos += hidSpecialKeys[i].width + 2;
    }
  }

  u8g2.sendBuffer();
}

void handleHIDSpecialKey(int index) {
  if (index >= 0 && index < hidSpecialKeysCount) {
    if (strcmp(hidSpecialKeys[index].label, "Space") == 0) {
      Keyboard.print(' ');
    } else {
      Keyboard.press(hidSpecialKeys[index].keyCode);
      delay(10);
      Keyboard.release(hidSpecialKeys[index].keyCode);
    }
  }
}



void hidkeyboard() {
  static unsigned long lastDebounceTime = 0;
  const int debounceDelay = 150;

  if (millis() - lastDebounceTime > debounceDelay) {
    if (isPressed(BTN_DOWN)) {
      if (!hidInSpecialKeys) {
        if (hidSelectedRow < hidRows - 1) {
          hidSelectedRow++;
          if (hidSelectedRow >= hidScrollOffset + hidVisibleRows) {
            hidScrollOffset++;
          }
        } else {
          hidInSpecialKeys = true;
          hidSelectedSpecial = 0;
        }
      } else {
        if (hidSelectedSpecial < hidSpecialKeysCount - 1) {
          hidSelectedSpecial++;
        }
      }
      lastDebounceTime = millis();
    }
    else if (isPressed(BTN_UP)) {
      if (hidInSpecialKeys) {
        if (hidSelectedSpecial > 0) {
          hidSelectedSpecial--;
        } else {
          hidInSpecialKeys = false;
          hidSelectedRow = hidRows - 1;
        }
      } else {
        if (hidSelectedRow > 0) {
          hidSelectedRow--;
          if (hidSelectedRow < hidScrollOffset) {
            hidScrollOffset--;
          }
        }
      }
      lastDebounceTime = millis();
    }
    else if (isPressed(BTN_RIGHT)) {
      if (hidInSpecialKeys) {
        if (hidSelectedSpecial < hidSpecialKeysCount - 1) {
          hidSelectedSpecial++;
        }
      } else {
        hidSelectedCol = (hidSelectedCol + 1) % hidCols;
      }
      lastDebounceTime = millis();
    }
    else if (isPressed(BTN_LEFT)) {
      if (hidInSpecialKeys) {
        if (hidSelectedSpecial > 0) {
          hidSelectedSpecial--;
        }
      } else {
        hidSelectedCol = (hidSelectedCol - 1 + hidCols) % hidCols;
      }
      lastDebounceTime = millis();
    }
    else if (isPressed(BTN_SELECT)) {
      if (hidInSpecialKeys) {
        handleHIDSpecialKey(hidSelectedSpecial);
      } else {
        char ch = hidKeyboard[hidSelectedRow][hidSelectedCol];
        Keyboard.print(ch);
      }
      lastDebounceTime = millis();
    }
    else if (isPressed(BTN_BACK)) {
      hidInSpecialKeys = !hidInSpecialKeys;
      lastDebounceTime = millis();
    }
  }

  drawHIDKeyboard();
}

