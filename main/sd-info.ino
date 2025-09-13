
// === Global Variables ===
float sdinfo_totalMB = 0, sdinfo_usedMB = 0, sdinfo_freeMB = 0;
int sdinfo_fileCount = 0;
uint64_t sdinfo_largestFile = 0, sdinfo_smallestFile = UINT64_MAX;
bool sdinfo_showDetails = false;

// === Init Display ===
void sdinfo_initDisplay() {
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(20, 30, "Initializing...");
  u8g2.sendBuffer();
}



// === Read SD Stats ===
void sdinfo_readStats() {
  File root = SD.open("/");
  sdinfo_usedMB = 0;
  sdinfo_fileCount = 0;
  sdinfo_largestFile = 0;
  sdinfo_smallestFile = UINT64_MAX;

  while (true) {
    File entry = root.openNextFile();
    if (!entry) break;
    if (!entry.isDirectory()) {
      uint64_t size = entry.size();
      sdinfo_usedMB += (float)size / (1024.0 * 1024.0);
      sdinfo_fileCount++;
      if (size > sdinfo_largestFile) sdinfo_largestFile = size;
      if (size < sdinfo_smallestFile) sdinfo_smallestFile = size;
    }
    entry.close();
  }
  root.close();

  sdinfo_totalMB = (float)SD.cardSize() / (1024.0 * 1024.0);
  sdinfo_freeMB = sdinfo_totalMB - sdinfo_usedMB;
}

// === Draw Main Info Screen ===
void sdinfo_drawMainScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(20, 10, "SD Card Info");

  u8g2.drawRBox(0, 14, 128, 14, 4);
  u8g2.setDrawColor(0);
  u8g2.setCursor(4, 24);
  u8g2.printf("Total: %.2f MB", sdinfo_totalMB);
  u8g2.setDrawColor(1);

  u8g2.drawRBox(0, 30, 128, 14, 4);
  u8g2.setDrawColor(0);
  u8g2.setCursor(4, 40);
  u8g2.printf("Used:  %.2f MB", sdinfo_usedMB);
  u8g2.setDrawColor(1);

  u8g2.drawRBox(0, 46, 128, 14, 4);
  u8g2.setDrawColor(0);
  u8g2.setCursor(4, 56);
  u8g2.printf("Free:  %.2f MB", sdinfo_freeMB);
  u8g2.setDrawColor(1);

  u8g2.sendBuffer();
}

// === Draw Detailed Info Screen ===
void sdinfo_drawDetailsScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, "Details:");

  u8g2.setCursor(0, 24);
  u8g2.printf("Files: %d", sdinfo_fileCount);

  u8g2.setCursor(0, 36);
  u8g2.printf("Largest: %.2f KB", (float)sdinfo_largestFile / 1024.0);

  u8g2.setCursor(0, 48);
  if (sdinfo_fileCount > 0)
    u8g2.printf("Smallest: %.2f KB", (float)sdinfo_smallestFile / 1024.0);
  else
    u8g2.print("Smallest: N/A");

  u8g2.setCursor(0, 60);
  u8g2.print("Press SELECT to back");

  u8g2.sendBuffer();
}

// === Button Handling ===
void sdinfo_handleButtons() {
  static bool lastSelect = HIGH;
  bool selectPressed = digitalRead(BTN_SELECT) == LOW;

  if (selectPressed && lastSelect == HIGH) {
    sdinfo_showDetails = !sdinfo_showDetails;
    if (sdinfo_showDetails)
      sdinfo_drawDetailsScreen();
    else
      sdinfo_drawMainScreen();
  }

  lastSelect = !selectPressed;
}

void showsdinfo() {
  sdinfo_handleButtons();
  delay(100);  // زرار debounce
}


