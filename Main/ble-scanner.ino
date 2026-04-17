bool blescanner_isPressed(int pin) {
  return digitalRead(pin) == LOW;
}

class blescanner_AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) override {
    blescanner_Device dev;
    
    
    std::string tempName = advertisedDevice.getName();
    
    
    if (tempName.empty() && advertisedDevice.haveServiceData()) {
      tempName = advertisedDevice.getServiceData();
    }
    
    
    if (tempName.empty()) {
      std::string addr = advertisedDevice.getAddress().toString();
      tempName = "BLE_" + addr.substr(addr.length() - 8);
    }
    
    dev.name = String(tempName.c_str());
    dev.address = String(advertisedDevice.getAddress().toString().c_str());
    dev.rssi = advertisedDevice.getRSSI();

   
    if (advertisedDevice.haveManufacturerData()) {
      std::string mData = advertisedDevice.getManufacturerData();
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

   
    if (advertisedDevice.haveServiceUUID()) {
      dev.deviceType = String(advertisedDevice.getServiceUUID().toString().c_str());
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
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(0, 45, "Press BACK to scan");
  } else {
   
    char counter[20];
    sprintf(counter, "%d/%d devices", blescanner_selectedIndex + 1, (int)blescanner_devices.size());
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(0, 8, counter);
    
    u8g2.setFont(u8g2_font_6x10_tr);
    int maxToShow = min(3, (int)blescanner_devices.size() - blescanner_selectedIndex);
    for (int i = 0; i < maxToShow; i++) {
      int idx = i + blescanner_selectedIndex;
      int y = 22 + i * 14;
      
      if (i == 0) {
        u8g2.drawRBox(0, y - 11, 128, 13, 2);
        u8g2.setDrawColor(0);
      }
      
    
      String displayText = blescanner_devices[idx].name;
      if (displayText.length() > 14) {
        displayText = displayText.substring(0, 14) + "..";
      }
      displayText += " (" + String(blescanner_devices[idx].rssi) + ")";
      
      u8g2.drawStr(2, y, displayText.c_str());
      
      if (i == 0) {
        u8g2.setDrawColor(1);
      }
    }
  }

  u8g2.sendBuffer();
}

void blescanner_drawDeviceDetails(const blescanner_Device& dev) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(0, 9, "Device Details:");
  u8g2.setFont(u8g2_font_5x7_tr);

  
  String name = dev.name;
  if (name.length() > 21) {
    u8g2.drawStr(0, 19, name.substring(0, 21).c_str());
    u8g2.drawStr(0, 27, name.substring(21).c_str());
  } else {
    String nameStr = "Name: " + name;
    u8g2.drawStr(0, 19, nameStr.c_str());
  }

  
  String macLine1 = "MAC: " + dev.address.substring(0, 12);
  String macLine2 = "     " + dev.address.substring(12);
  u8g2.drawStr(0, 35, macLine1.c_str());
  u8g2.drawStr(0, 43, macLine2.c_str());

  char rssiStr[25];
  sprintf(rssiStr, "RSSI: %d dBm", dev.rssi);
  u8g2.drawStr(0, 51, rssiStr);

 
  String deviceType = dev.deviceType;
  if (deviceType.length() > 21) {
    deviceType = deviceType.substring(0, 18) + "...";
  }
  String typeStr = "Type: " + deviceType;
  u8g2.drawStr(0, 59, typeStr.c_str());

  u8g2.sendBuffer();
}

void blescanner_scan() {
  blescanner_devices.clear();
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x14_tr);
  u8g2.drawStr(10, 30, "Scanning BLE...");
  u8g2.sendBuffer();
  
  BLEDevice::init("");
  blescanner_pBLEScan = BLEDevice::getScan();
  blescanner_pBLEScan->setAdvertisedDeviceCallbacks(new blescanner_AdvertisedDeviceCallbacks(), false);
  blescanner_pBLEScan->setActiveScan(true);
  blescanner_pBLEScan->setInterval(100);
  blescanner_pBLEScan->setWindow(99);
  blescanner_pBLEScan->start(5, false);
  
  // إزالة التكرارات
  std::vector<blescanner_Device> uniqueDevices;
  for (size_t i = 0; i < blescanner_devices.size(); i++) {
    bool exists = false;
    for (size_t j = 0; j < uniqueDevices.size(); j++) {
      if (uniqueDevices[j].address == blescanner_devices[i].address) {
        exists = true;
        break;
      }
    }
    if (!exists) {
      uniqueDevices.push_back(blescanner_devices[i]);
    }
  }
  blescanner_devices = uniqueDevices;
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
      delay(4000);
      blescanner_drawMenu();
      lastPress = millis();
    } else if (blescanner_isPressed(BTN_BACK)) {
      blescanner_scan();
      blescanner_selectedIndex = 0;
      blescanner_drawMenu();
      lastPress = millis();
    }
  }
}