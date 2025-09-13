//////////sd flasher tool variables///////////////
//////////////////////////////////////////////////
// Files
#define MAX_FILES 50
String files[MAX_FILES];
int fileCount = 0;
int selectedIndex = 0;
int topIndex = 0;

bool confirmFlash = false;  // وضع تأكيد الفلاش


void flasherloop(){
  static uint32_t lastBtn = 0;
  if (millis() - lastBtn < 150) return;

  if (!digitalRead(BTN_UP) && !confirmFlash) {
    if (selectedIndex > 0) {
      selectedIndex--;
      if (selectedIndex < topIndex) topIndex--;
    }
    drawMenu();
    lastBtn = millis();
  }

  if (!digitalRead(BTN_DOWN) && !confirmFlash) {
    if (selectedIndex < fileCount - 1) {
      selectedIndex++;
      if (selectedIndex >= topIndex + 6) topIndex++;
    }
    drawMenu();
    lastBtn = millis();
  }

  if (!digitalRead(BTN_SELECT)) {
    if (confirmFlash) {
      String filename = files[selectedIndex];
      confirmFlash = false;
      flashBinary(filename);
    } else {
      String filename = files[selectedIndex];
      if (filename.endsWith(".bin")) {
        confirmFlash = true;
        drawMenu();
      }
    }
    lastBtn = millis();
  }

  if (!digitalRead(BTN_BACK)) {
    if (confirmFlash) {
      confirmFlash = false;
      drawMenu();
    }
    lastBtn = millis();
  }
}







void readSDfiles() {
  fileCount = 0;
  File root = SD.open("/");
  while (true) {
    File entry = root.openNextFile();
    if (!entry || fileCount >= MAX_FILES) break;
    if (!entry.isDirectory()) {
      files[fileCount++] = String(entry.name());
    }
    entry.close();
  }
}

void drawMenu() {
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);

  if (confirmFlash) {
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(0, 20, "Flash selected file?");
    u8g2.drawStr(0, 35, "SELECT = Yes");
    u8g2.drawStr(0, 50, "BACK = Cancel");
    u8g2.sendBuffer();
    return;
  }

  for (int i = 0; i < 6; i++) {
    int index = topIndex + i;
    if (index >= fileCount) break;

    if (index == selectedIndex) {
      u8g2.drawBox(0, i * 10, 128, 10);
      u8g2.setDrawColor(0);
    } else {
      u8g2.setDrawColor(1);
    }

    u8g2.setCursor(2, (i + 1) * 10 - 2);
    u8g2.print(files[index]);
  }
  u8g2.sendBuffer();
}

void flashBinary(String path) {
  File updateBin = SD.open("/" + path);
  if (!updateBin) {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 30, "File not found!");
    u8g2.sendBuffer();
    delay(2000);
    return;
  }

  if (updateBin.isDirectory()) {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 30, "Not a file!");
    u8g2.sendBuffer();
    delay(2000);
    return;
  }

  size_t updateSize = updateBin.size();

  u8g2.clearBuffer();
  u8g2.drawStr(0, 30, "Flashing...");
  u8g2.sendBuffer();

  if (!Update.begin(updateSize)) {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 30, "Update.begin fail");
    u8g2.sendBuffer();
    return;
  }

  size_t written = Update.writeStream(updateBin);
  if (written == updateSize && Update.end(true)) {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 20, "Update success!");
    u8g2.drawStr(0, 40, "Rebooting...");
    u8g2.sendBuffer();
    delay(2000);
    ESP.restart();
  } else {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 20, "Update failed!");
    u8g2.sendBuffer();
    delay(2000);
  }

  updateBin.close();
}
