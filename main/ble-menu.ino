void handleblemenu() {
  const char* menuItems[] = {"ble scanner","BLE MOUSE", "BLE KEYBOARD", "BLE SCRIPT"};
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
  static const unsigned char image_bluetooth_bits[] U8X8_PROGMEM = {0x80,0x00,0x40,0x01,0x40,0x02,0x44,0x04,0x48,0x04,0x50,0x02,0x60,0x01,0xc0,0x00,0x60,0x01,0x50,0x02,0x48,0x04,0x44,0x04,0x40,0x02,0x40,0x01,0x80,0x00,0x00,0x00};
static const unsigned char image_file_search_bits[] U8X8_PROGMEM = {0x80,0x0f,0x40,0x10,0x20,0x20,0x10,0x40,0x10,0x40,0x10,0x50,0x10,0x50,0x10,0x48,0x20,0x26,0x50,0x10,0xa8,0x0f,0x14,0x00,0x0a,0x00,0x05,0x00,0x03,0x00,0x00,0x00};


    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    // Layer 1
    u8g2.setFont(u8g2_font_t0_13_tr);
    u8g2.drawStr(34, 16, "scanning");

    // bluetooth
    u8g2.drawXBMP(100, 22, 14, 16, image_bluetooth_bits);

    // Layer 3
    u8g2.drawStr(51, 34, "ble ");

    // file_search
    u8g2.drawXBMP(11, 22, 15, 16, image_file_search_bits);

    // Layer 5
    u8g2.drawStr(40, 50, "devices");

    u8g2.sendBuffer();



  blescanner_scan();
  blescanner_selectedIndex = 0;
  blescanner_drawMenu();
      runLoop(blescanner);
        break;
      case 1:
      mouse_ble.begin();
      runLoop(blemouse);
       
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
