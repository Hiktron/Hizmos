// Namespace for RF jamming control and menu
namespace nrf {
  bool stopJamming = false;

  const char* menuItems[] = {
    "Bluetooth",  // 0-78
    "Zigbee",     // 15-26 (channels 11-26)
    "Wi-Fi",      // 10-84 (ch 1-13)
    "Drones",     // 40-70 (based on DJI/FPV)
    "All"
  };

  const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
  int currentPage = 0;
}




void drawMenuPage(int index) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso16_tr);

  // Title centered
  int strWidth = u8g2.getStrWidth(nrf::menuItems[index]);
  u8g2.drawStr((128 - strWidth) / 2, 35, nrf::menuItems[index]);

  // Bottom indicator
  for (int i = 0; i < nrf::menuLength; i++) {
    int x = 20 + i * 15;
    if (i == index)
      u8g2.drawDisc(x, 60, 3);  // filled
    else
      u8g2.drawCircle(x, 60, 3); // hollow
  }

  u8g2.sendBuffer();
}

void jamChannels(const char* label, int startCh, int endCh) {
  byte data1[32], data2[32];
  for (int i = 0; i < 32; i++) {
    data1[i] = random(0, 256);
    data2[i] = random(0, 256);
  }

  for (int ch = startCh; ch <= endCh && !nrf::stopJamming; ch++) {
    // Status screen
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(0, 10, "Jamming:");
    u8g2.setCursor(60, 10);
    u8g2.print(label);
    u8g2.setCursor(0, 30);
    u8g2.print("Channel: ");
    u8g2.print(ch);
    u8g2.sendBuffer();

    unsigned long startTime = millis();
    while (millis() - startTime < JAM_DURATION) {
      if (digitalRead(BTN_BACK) == LOW) {
        nrf::stopJamming = true;
        return;
      }
      radio1.setChannel(ch);
      radio1.stopListening();
      radio1.write(data1, sizeof(data1));
      delayMicroseconds(100);

      radio2.setChannel(ch);
      radio2.stopListening();
      radio2.write(data2, sizeof(data2));
      delayMicroseconds(100);
    }
  }
}

void handleSelection(int item) {
  nrf::stopJamming = false;

  switch (item) {
    case 0:
      jamChannels("Bluetooth", 0, 78);
      break;
    case 1:
      jamChannels("Zigbee", 15, 26);
      break;
    case 2:
      jamChannels("Wi-Fi", 10, 84);
      break;
    case 3:
      jamChannels("Drones", 40, 70);
      break;
    case 4:
      jamChannels("All", 0, 84);
      break;
  }

  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, nrf::stopJamming ? "Stopped" : "Done");
  u8g2.sendBuffer();
  delay(1000);
}




void nrfjammer() {
  if (digitalRead(BTN_LEFT) == LOW) {
    nrf::currentPage = (nrf::currentPage - 1 + nrf::menuLength) % nrf::menuLength;
    drawMenuPage(nrf::currentPage);
    delay(200);
  }

  if (digitalRead(BTN_RIGHT) == LOW) {
    nrf::currentPage = (nrf::currentPage + 1) % nrf::menuLength;
    drawMenuPage(nrf::currentPage);
    delay(200);
  }

  if (digitalRead(BTN_SELECT) == LOW) {
    handleSelection(nrf::currentPage);
    drawMenuPage(nrf::currentPage);
    delay(200);
  }
}


