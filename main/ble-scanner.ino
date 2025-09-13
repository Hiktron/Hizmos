
bool blescanner_isPressed(int pin) {
  return digitalRead(pin) == LOW;
}

class blescanner_AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) override {
    blescanner_Device dev;
    
    dev.name = advertisedDevice.getName().c_str();
    if (dev.name == "") dev.name = "(no name)";
    dev.address = advertisedDevice.getAddress().toString().c_str();
    dev.rssi = advertisedDevice.getRSSI();

    // مصنع الجهاز
    if (advertisedDevice.haveManufacturerData()) {
      String mData = advertisedDevice.getManufacturerData();

      if (mData.length() >= 2) {
        char buffer[10];
        sprintf(buffer, "0x%02X%02X", (uint8_t)mData[1], (uint8_t)mData[0]);
        dev.manufacturer = String(buffer);
      } else {
        dev.manufacturer = "unknown";
      }
    } else {
      dev.manufacturer = "unknown";
    }

    // نوع الجهاز
    if (advertisedDevice.haveServiceUUID()) {
      dev.deviceType = advertisedDevice.getServiceUUID().toString().c_str();
    } else {
      dev.deviceType = "unknown";
    }

    blescanner_devices.push_back(dev);
  }
};

void blescanner_drawMenu() {
  setColor(0,0,0);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x14_tr);

  if (blescanner_devices.empty()) {
    u8g2.drawStr(0, 30, "No devices found.");
  } else {
    int maxToShow = min(3, (int)blescanner_devices.size() - blescanner_selectedIndex);
    for (int i = 0; i < maxToShow; i++) {
      int idx = i + blescanner_selectedIndex;
      int y = 16 + i * 18;
      if (i == 0) {
        u8g2.drawRBox(0, y - 14, 128, 18, 4);
        u8g2.setDrawColor(0);
        u8g2.drawStr(5, y, blescanner_devices[idx].name.c_str());
        u8g2.setDrawColor(1);
      } else {
        u8g2.drawStr(5, y, blescanner_devices[idx].name.c_str());
      }
    }
  }

  u8g2.sendBuffer();
}

void blescanner_drawDeviceDetails(const blescanner_Device& dev) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(0, 10, "Device Info:");
  u8g2.setFont(u8g2_font_5x8_tr);

  u8g2.drawStr(0, 22, ("Name: " + dev.name).c_str());
  u8g2.drawStr(0, 32, ("MAC: " + dev.address).c_str());

  char rssiStr[20];
  sprintf(rssiStr, "RSSI: %d dBm", dev.rssi);
  u8g2.drawStr(0, 42, rssiStr);

  u8g2.drawStr(0, 52, ("Type: " + dev.deviceType).c_str());
  u8g2.drawStr(0, 62, ("Manuf: " + dev.manufacturer).c_str());

  u8g2.sendBuffer();
}

void blescanner_scan() {
  blescanner_devices.clear();
  BLEDevice::init("");
  blescanner_pBLEScan = BLEDevice::getScan();
  blescanner_pBLEScan->setAdvertisedDeviceCallbacks(new blescanner_AdvertisedDeviceCallbacks(), false);
  blescanner_pBLEScan->setActiveScan(true);
  blescanner_pBLEScan->start(5, false); // scan for 5 seconds
}

void blescanner() {
  static unsigned long lastPress = 0;

  if (millis() - lastPress > 200) {
    if (blescanner_isPressed(BTN_DOWN) && blescanner_selectedIndex < (int)blescanner_devices.size() - 1) {
      blescanner_selectedIndex++;
      blescanner_drawMenu();
      lastPress = millis();
    } else if (blescanner_isPressed(BTN_UP) && blescanner_selectedIndex > 0) {
      blescanner_selectedIndex--;
      blescanner_drawMenu();
      lastPress = millis();
    } else if (blescanner_isPressed(BTN_SELECT) && !blescanner_devices.empty()) {
      blescanner_drawDeviceDetails(blescanner_devices[blescanner_selectedIndex]);
      delay(3000);
      blescanner_drawMenu();
    } else if (blescanner_isPressed(BTN_BACK)) {
      u8g2.clearBuffer();
      u8g2.drawStr(0, 20, "Rescanning...");
      u8g2.sendBuffer();
      blescanner_scan();
      blescanner_selectedIndex = 0;
      blescanner_drawMenu();
      lastPress = millis();
    }
  }
}




