
static const uint8_t image_badusb_10px_bits[] U8X8_PROGMEM = {0x00,0x00,0xf8,0x00,0xfc,0x01,0x74,0x01,0x24,0x01,0xfc,0x01,0xfc,0x01,0xa8,0x00,0x00,0x00,0x00,0x00};
static const uint8_t image_dir_10px_bits[]     U8X8_PROGMEM = {0x00,0x00,0x3f,0x00,0x41,0x00,0xff,0x03,0x01,0x02,0x01,0x02,0x01,0x02,0x01,0x02,0xfe,0x01,0x00,0x00};
static const uint8_t image_ir_10px_bits[]      U8X8_PROGMEM = {0xfc,0x00,0x02,0x01,0x79,0x02,0x84,0x00,0x30,0x00,0x00,0x00,0x30,0x00,0x58,0x00,0x78,0x00,0xff,0x03};
static const uint8_t image_Nfc_10px_bits[]     U8X8_PROGMEM = {0x80,0x00,0x00,0x01,0x22,0x02,0x43,0x02,0x45,0x02,0x49,0x02,0x31,0x02,0x22,0x02,0x00,0x01,0x80,0x00};
static const uint8_t image_sub1_10px_bits[]    U8X8_PROGMEM = {0x02,0x01,0x49,0x02,0x85,0x02,0xb5,0x02,0xb5,0x02,0x01,0x02,0x32,0x01,0x30,0x00,0x30,0x00,0x30,0x00};
static const uint8_t image_unknown_10px_bits[] U8X8_PROGMEM = {0x78,0x00,0xcc,0x00,0x84,0x00,0xc0,0x00,0x60,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x30,0x00,0x30,0x00};
static const uint8_t image_update_10px_bits[]  U8X8_PROGMEM = {0x00,0x00,0x3c,0x00,0x7e,0x00,0xdb,0x00,0x99,0x00,0x99,0x00,0xdb,0x00,0x7e,0x00,0x3c,0x00,0x00,0x00};
File filoRoot;
String filoFileList[50];
int filoFileCount = 0;
int filoSelectedIndex = 0;
int filoTopIndex = 0;

bool filoContextMenuActive = false;
int filoContextIndex = 0;
const char* filoContextOptions[] = {"Open File", "Rename File", "Delete File"};

bool filoFileViewerActive = false;
File filoViewingFile;
int filoViewLineOffset = 0;




String inputreName() {
  const char* keys[] = {
    "<","A","B","C","D","E","F","G",
    "H","I","J","K","L","M","N","O",
    "P","Q","R","S","T","U","V","W",
    "X","Y","Z","0","1","2","3","4",
    "5","6","7","8","9","-","_","*",
    "/","@","#",".",",","ENTER"
  };
  const int totalKeys = sizeof(keys) / sizeof(keys[0]);
  const int cols = 8;
  const int rows = (totalKeys + cols - 1) / cols;
  const int visibleRows = 3;
  const int maxNameLen = 12;

  int offsetY = 0;
  int cursorX = 0, cursorY = 0;
  String name = "";

  while (true) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tf);

    u8g2.drawStr(0, 10, "Rename to:");
    u8g2.drawStr(70, 10, name.c_str());

    for (int y = 0; y < visibleRows; y++) {
      for (int x = 0; x < cols; x++) {
        int keyIndex = (offsetY + y) * cols + x;
        if (keyIndex >= totalKeys) continue;

        int boxX = x * 16;
        int boxY = 14 + y * 16;
        const char* key = keys[keyIndex];
        int boxW = (strcmp(key, "ENTER") == 0) ? 30 : 14;

        if (x == cursorX && y == cursorY) {
          u8g2.drawBox(boxX, boxY, boxW, 14);
          u8g2.setDrawColor(0);
          u8g2.setCursor(boxX + 2, boxY + 11);
          u8g2.print(key);
          u8g2.setDrawColor(1);
        } else {
          u8g2.drawFrame(boxX, boxY, boxW, 14);
          u8g2.setCursor(boxX + 2, boxY + 11);
          u8g2.print(key);
        }
      }
    }

    u8g2.sendBuffer();

    if (digitalRead(BTN_UP) == LOW) {
      if (cursorY == 0 && offsetY > 0) offsetY--;
      else if (cursorY > 0) cursorY--;
      delay(150);
    }

    if (digitalRead(BTN_DOWN) == LOW) {
      if ((cursorY + offsetY + 1) * cols + cursorX < totalKeys && cursorY < visibleRows - 1) cursorY++;
      else if (offsetY + visibleRows < rows) offsetY++;
      delay(150);
    }

    if (digitalRead(BTN_LEFT) == LOW && cursorX > 0) {
      cursorX--;
      delay(150);
    }

    if (digitalRead(BTN_RIGHT) == LOW && cursorX < cols - 1) {
      if ((offsetY + cursorY) * cols + cursorX + 1 < totalKeys) cursorX++;
      delay(150);
    }

    if (digitalRead(BTN_SELECT) == LOW) {
      int keyIndex = (cursorY + offsetY) * cols + cursorX;
      if (keyIndex < totalKeys) {
        const char* key = keys[keyIndex];
        if (strcmp(key, "<") == 0) {
          if (name.length() > 0) name.remove(name.length() - 1);
        } else if (strcmp(key, "ENTER") == 0) {
          return name;
        } else {
          if (name.length() < maxNameLen) name += key;
        }
      }
      delay(200);
    }

    if (digitalRead(BTN_BACK) == LOW) {
      return ""; // cancel
    }
  }
}



void listFiles(File dir) {
  filoFileCount = 0;
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;
    String name = entry.name();
    if (!entry.isDirectory()) {
      filoFileList[filoFileCount++] = name;
    }
    entry.close();
  }
}

const uint8_t* getFileIcon(String name) {
  if (name.endsWith(".ir") || name.endsWith(".IR")) return image_ir_10px_bits;
  else if (name.endsWith(".nfc")) return image_Nfc_10px_bits;
  else if (name.endsWith(".bad") || name.endsWith(".duck")) return image_badusb_10px_bits;
  else if (name.endsWith(".sub")) return image_sub1_10px_bits;
  else if (name.endsWith(".bin")) return image_update_10px_bits;
  else return image_unknown_10px_bits;
}

void drawFileMenu() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);

  for (int i = 0; i < 5; i++) {
    int idx = filoTopIndex + i;
    if (idx >= filoFileCount) break;

    if (idx == filoSelectedIndex) {
      u8g2.drawRBox(0, i * 12, 122, 12, 3);  // قللنا العرض علشان يبان السكروول
      u8g2.setDrawColor(0);
    }

    u8g2.drawXBMP(0, i * 12 + 1, 10, 10, getFileIcon(filoFileList[idx]));
    u8g2.setCursor(14, i * 12 + 10);
    u8g2.print(filoFileList[idx]);

    if (idx == filoSelectedIndex) u8g2.setDrawColor(1);
  }

  // Scroll Track + Indicator
  if (filoFileCount > 5) {
    int scrollbarHeight = 60;
    int indicatorHeight = max(60 * 5 / filoFileCount, 8);
    int indicatorPos = map(filoTopIndex, 0, filoFileCount - 5, 0, scrollbarHeight - indicatorHeight);

    // الخط الخلفي (التراك)
    u8g2.drawVLine(126, 2, scrollbarHeight);  // خط ثابت على طول التراك

    // المؤشر
    u8g2.drawBox(124, 2 + indicatorPos, 3, indicatorHeight);  // البوكس نفسه
  }

  u8g2.sendBuffer();
}




void drawContextMenu() {
  const int itemHeight = 12;
  const int menuX = 0;
  const int menuWidth = 128;
  const int visibleItems = 3;
  const int totalItems = sizeof(filoContextOptions) / sizeof(filoContextOptions[0]);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);

  for (int i = 0; i < visibleItems; i++) {
    if (i == filoContextIndex) {
      u8g2.drawRBox(menuX, i * itemHeight, menuWidth, itemHeight, 3);
      u8g2.setDrawColor(0);
    }

    u8g2.setCursor(menuX + 5, i * itemHeight + 10);
    u8g2.print(filoContextOptions[i]);

    if (i == filoContextIndex) {
      u8g2.setDrawColor(1);
    }
  }

  // Scroll indicator (bar on the right)
  if (totalItems > visibleItems) {
    int scrollBarHeight = (visibleItems * itemHeight * visibleItems) / totalItems;
    int scrollBarY = (itemHeight * filoContextIndex * visibleItems) / totalItems;
    u8g2.drawBox(menuWidth - 3, scrollBarY, 2, scrollBarHeight);
  }

  u8g2.sendBuffer();
}

void drawFileViewer() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_5x8_tf);
  filoViewingFile.seek(0);
  int currentLine = 0;
  while (filoViewingFile.available()) {
    String line = filoViewingFile.readStringUntil('\n');
    if (currentLine >= filoViewLineOffset && currentLine < filoViewLineOffset + 7) {
      u8g2.setCursor(0, (currentLine - filoViewLineOffset + 1) * 9);
      u8g2.print(line);
    }
    currentLine++;
  }
  u8g2.sendBuffer();
}

void filemenu() {
  if (filoFileViewerActive) {
    drawFileViewer();
    if (digitalRead(BTN_BACK) == LOW) {
      filoFileViewerActive = false;
      filoViewingFile.close();
      delay(200);
    }
    if (digitalRead(BTN_UP) == LOW) {
      if (filoViewLineOffset > 0) filoViewLineOffset--;
      delay(150);
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      filoViewLineOffset++;
      delay(150);
    }
    return;
  }

  if (filoContextMenuActive) {
    drawContextMenu();
    if (digitalRead(BTN_UP) == LOW) {
      if (filoContextIndex > 0) filoContextIndex--;
      delay(150);
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      if (filoContextIndex < 2) filoContextIndex++;
      delay(150);
    }
    if (digitalRead(BTN_SELECT) == LOW) {
      String selectedFile = filoFileList[filoSelectedIndex];
      if (filoContextIndex == 0) {
        filoViewingFile = SD.open("/" + selectedFile);
        if (filoViewingFile) {
          filoViewLineOffset = 0;
          filoFileViewerActive = true;
        }
      } else if (filoContextIndex == 1) {
        String newName = inputreName();
        if (newName != "") {
          newName += ".txt";
          if (SD.exists("/" + newName)) SD.remove("/" + newName);
          SD.rename("/" + selectedFile, "/" + newName);
          filoRoot.rewindDirectory();
          listFiles(filoRoot);
          filoSelectedIndex = 0;
          filoTopIndex = 0;
        }
      } else if (filoContextIndex == 2) {
        // تأكيد الحذف
        u8g2.clearBuffer();
        u8g2.setCursor(0, 20);
        u8g2.print("Delete?");
        u8g2.setCursor(0, 35);
        u8g2.print(selectedFile);
        u8g2.setCursor(0, 55);
        u8g2.print("Press SELECT to confirm");
        u8g2.sendBuffer();
        while (true) {
          delay(400);
          if (digitalRead(BTN_SELECT) == LOW) {
            SD.remove("/" + selectedFile);
            filoRoot.rewindDirectory();
            listFiles(filoRoot);
            filoSelectedIndex = 0;
            filoTopIndex = 0;
            break;
          }
          if (digitalRead(BTN_BACK) == LOW) break;
        }
      }
      filoContextMenuActive = false;
      delay(200);
    }
    if (digitalRead(BTN_BACK) == LOW) {
      filoContextMenuActive = false;
      delay(200);
    }
    return;
  }

  drawFileMenu();

  if (digitalRead(BTN_UP) == LOW) {
    if (filoSelectedIndex > 0) filoSelectedIndex--;
    if (filoSelectedIndex < filoTopIndex) filoTopIndex--;
    delay(150);
  }

  if (digitalRead(BTN_DOWN) == LOW) {
    if (filoSelectedIndex < filoFileCount - 1) filoSelectedIndex++;
    if (filoSelectedIndex >= filoTopIndex + 5) filoTopIndex++;
    delay(150);
  }

  if (digitalRead(BTN_SELECT) == LOW) {
    filoContextMenuActive = true;
    filoContextIndex = 0;
    delay(200);
  }
}


void filesetup() {

if (!SD.begin(SD_CS, SD_SPI)) {
 runLoop(drawnosdcard);
  }

  filoRoot = SD.open("/");
  listFiles(filoRoot);





}

  