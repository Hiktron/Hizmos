void handlesubghzmenu() {
  const char* menuItems[] = {"SPECTRUM GRAPGH","READ", "READ RAW", "FREQUENCY ANALYZER", "JAMMER","BRUTE FORCE","CONFIG", "SAVED SIGNALS"};
  const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
  const int visibleItems = 3;

  static int selectedItem = 0;
  static int scrollOffset = 0;

 
  static unsigned long lastInputTime = 0;

  
  if (millis() - lastInputTime > 150) {

 
  if (digitalRead(BTN_UP) == LOW) {
    selectedItem--;
    if (selectedItem < 0) selectedItem = menuLength - 1;
    scrollOffset = constrain(selectedItem - visibleItems + 1, 0, menuLength - visibleItems);
    lastInputTime = millis(); 
  }

  if (digitalRead(BTN_DOWN) == LOW) {
    selectedItem++;
    if (selectedItem >= menuLength) selectedItem = 0;
    scrollOffset = constrain(selectedItem - visibleItems + 1, 0, menuLength - visibleItems);
    lastInputTime = millis(); 
  }

  if (digitalRead(BTN_SELECT) == LOW) {
    switch (selectedItem) {
      case 0:
       ///spectrum graph
       //lazyinit();
       lazyInitCC1101(1); // شغل CC1101 الأول
       runLoop(drawHistogram);
        break;
      case 1:
       ///read 
       break;
      case 2:
      
        break;
      case 3: 
       ///spectrum analyzer
       //lazyinit();
       //lazyInit(2); // شغل CC1101 الأول
       lazyInitCC1101(1);
       runLoop(SPECTRUMANALYZER);
       break;

       case 4:
       ///////jammer///////
       //lazyinit();
       //lazyInit(2); // شغل CC1101 الأول
       lazyInitCC1101(1);
       runLoop(JAMMINGCC1101);
      
       break;

       case 5:
       //lazyinit();
       //lazyInit(2); // شغل CC1101 الأول
       lazyInitCC1101(2);
       runLoop(drawBruteForce);

       break;

       case 6:
       //lazyinit();
       lazyInitCC1101(2);
       runLoop(drawSettingsMenu);
       break;
      
    }
    lastInputTime = millis(); 
  }
  }
  // ===== عرض الشاشة =====
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x14_tf); 

  for (int i = 0; i < visibleItems; i++) {
    int menuIndex = i + scrollOffset;
    if (menuIndex >= menuLength) break;

    int y = i * 20 + 16;

    if (menuIndex == selectedItem) {
      u8g2.drawRBox(4, y - 12, 120, 16, 4); 
      u8g2.setDrawColor(0); 
      u8g2.drawStr(10, y, menuItems[menuIndex]);
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(10, y, menuItems[menuIndex]);
    }
  }


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


