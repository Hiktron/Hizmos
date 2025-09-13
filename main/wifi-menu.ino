void handlewifimenu() {
  const char* menuItems[] = {"SCAN WIFI", "PACKET ANALYZER", "BEACON", "CAPTIVE PORTAL", "DEAUTH DETECTOR"};
  const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
  const int visibleItems = 3;

  static int selectedItem = 0;
  static int scrollOffset = 0;

  // ✅ أضفنا ده لتتبع آخر وقت حصل فيه ضغط على زر
  static unsigned long lastInputTime = 0;

  // ✅ هنا بنشوف هل فات وقت كافي (150ms) من آخر ضغط
  if (millis() - lastInputTime > 150) {
    

  // ===== التعامل مع الأزرار =====
  if (digitalRead(BTN_UP) == LOW) {
    selectedItem--;
    if (selectedItem < 0) selectedItem = menuLength - 1;
    scrollOffset = constrain(selectedItem - visibleItems + 1, 0, menuLength - visibleItems);
    lastInputTime = millis(); // ✅ حدثنا الوقت بعد الضغط
  }

  if (digitalRead(BTN_DOWN) == LOW) {
    selectedItem++;
    if (selectedItem >= menuLength) selectedItem = 0;
    scrollOffset = constrain(selectedItem - visibleItems + 1, 0, menuLength - visibleItems);
    lastInputTime = millis(); // ✅ حدثنا الوقت بعد الضغط
  }

  if (digitalRead(BTN_SELECT) == LOW) {
    switch (selectedItem) {
      case 0:

     static const unsigned char image_file_search_bits[] U8X8_PROGMEM = {0x80,0x0f,0x40,0x10,0x20,0x20,0x10,0x40,0x10,0x40,0x10,0x50,0x10,0x50,0x10,0x48,0x20,0x26,0x50,0x10,0xa8,0x0f,0x14,0x00,0x0a,0x00,0x05,0x00,0x03,0x00,0x00,0x00};
static const unsigned char image_wifi_50_bits[] U8X8_PROGMEM = {0x80,0x0f,0x00,0x60,0x30,0x00,0x18,0xc0,0x00,0x84,0x0f,0x01,0x62,0x30,0x02,0x11,0x40,0x04,0x0a,0x87,0x02,0xc4,0x1f,0x01,0xe8,0xb8,0x00,0x70,0x77,0x00,0xa0,0x2f,0x00,0xc0,0x1d,0x00,0x80,0x0a,0x00,0x00,0x07,0x00,0x00,0x02,0x00,0x00,0x00,0x00};


    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    // file_search
    u8g2.drawXBMP(10, 21, 15, 16, image_file_search_bits);

    // wifi_50
    u8g2.drawXBMP(99, 20, 19, 16, image_wifi_50_bits);

    // Layer 3
    u8g2.setFont(u8g2_font_t0_13_tr);
    u8g2.drawStr(32, 15, "scanning");

    // Layer 4
    u8g2.drawStr(44, 32, "wifi ");

    // Layer 5
    u8g2.drawStr(33, 49, "networks");

    u8g2.sendBuffer();



       WiFi.mode(WIFI_STA);
       WiFi.disconnect();
       delay(100);
       wifi_networkCount = WiFi.scanNetworks();

      runLoop(scanningwifi);
       
        break;
      case 1:
       setupSnifferGraph(); 
       runLoop(updateSnifferGraph);
       break;
      case 2:
       runLoop(loading);
        break;
      case 3: 
      runLoop(loading);
       break;
      case 4:
      runLoop(loading);
        break;
    }
    lastInputTime = millis(); // ✅ حدثنا الوقت بعد الضغط
  }
  }
  // ===== عرض الشاشة =====
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x14_tf); // Nice clean font

  for (int i = 0; i < visibleItems; i++) {
    int menuIndex = i + scrollOffset;
    if (menuIndex >= menuLength) break;

    int y = i * 20 + 16;

    if (menuIndex == selectedItem) {
      u8g2.drawRBox(4, y - 12, 120, 16, 4); // Rounded highlight
      u8g2.setDrawColor(0); // black text on white box
      u8g2.drawStr(10, y, menuItems[menuIndex]);
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(10, y, menuItems[menuIndex]);
    }
  }

  // ===== شريط التمرير =====
  int barX = 124;
  int spacing = 64 / menuLength;

  for (int i = 0; i < menuLength; i++) {
    int dotY = i * spacing + spacing / 2;
    if (i == selectedItem) {
      u8g2.drawBox(barX, dotY - 3, 3, 6);
    } else {
      u8g2.drawPixel(barX + 1, dotY);
    }
  }

  u8g2.sendBuffer();
  
}
