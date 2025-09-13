void handlesuniversalremotemenu() {
  const char* menuItems[] = {"TV", "AC", "PROJECTOR", "CAMERAS"};
  const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
  const int visibleItems = 3;

  static int selectedItem = 0;
  static int scrollOffset = 0;

  // ✅ أضفنا ده لتتبع آخر وقت حصل فيه ضغط على زر
  static unsigned long lastInputTime = 0;

  // ✅ هنا بنشوف هل فات وقت كافي (150ms) من آخر ضغط
  if (millis() - lastInputTime > 150) {
    
    // ⬆️ زر UP
    if (digitalRead(BTN_UP) == LOW) {
      selectedItem--;
      if (selectedItem < 0) selectedItem = menuLength - 1;
      scrollOffset = constrain(selectedItem - visibleItems + 1, 0, menuLength - visibleItems);

      lastInputTime = millis(); // ✅ حدثنا الوقت بعد الضغط
    }

    // ⬇️ زر DOWN
    if (digitalRead(BTN_DOWN) == LOW) {
      selectedItem++;
      if (selectedItem >= menuLength) selectedItem = 0;
      scrollOffset = constrain(selectedItem - visibleItems + 1, 0, menuLength - visibleItems);

      lastInputTime = millis(); // ✅ نفس الفكرة هنا
    }

    // ✔️ زر SELECT
    if (digitalRead(BTN_SELECT) == LOW) {
      switch (selectedItem) {
        case 0: runLoop(loading); break;
        case 1: runLoop(loading); break;
        case 2: runLoop(loading); break;
        case 3:  runLoop(loading);break;
        case 4: runLoop(loading); break;
      }

      lastInputTime = millis(); // ✅ علشان مفيش ضغط جديد يتسجل فورًا بعد الدخول
    }
  }

  // ===== عرض القائمة على الشاشة =====
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x14_tf);

  for (int i = 0; i < visibleItems; i++) {
    int menuIndex = i + scrollOffset;
    if (menuIndex >= menuLength) break;

    int y = i * 20 + 16;
    if (menuIndex == selectedItem) {
      u8g2.drawRBox(4, y - 12, 120, 16, 4); // ✅ مستطيل تحديد العنصر
      u8g2.setDrawColor(0); // ✅ عكس اللون للنص المختار
      u8g2.drawStr(10, y, menuItems[menuIndex]);
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(10, y, menuItems[menuIndex]);
    }
  }

  // ===== شريط التمرير الجانبي (نقط) =====
  int barX = 124;
  int spacing = 64 / menuLength;

  for (int i = 0; i < menuLength; i++) {
    int dotY = i * spacing + spacing / 2;
    if (i == selectedItem) {
      u8g2.drawBox(barX, dotY - 3, 3, 6); // ✅ نقطة أكبر للعنصر المحدد
    } else {
      u8g2.drawPixel(barX + 1, dotY); // ✅ نقطة صغيرة لباقي العناصر
    }
  }

  u8g2.sendBuffer(); // ✅ إرسال الرسمة للشاشة
}
