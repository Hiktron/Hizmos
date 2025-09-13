 void datausage(){
  // Get RAM info
  size_t freeHeap = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
  size_t totalHeap = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
  int ramUsage = 100 - ((freeHeap * 100) / totalHeap);

  // Get Flash info
  uint32_t flashSize = ESP.getFlashChipSize();

  uint32_t flashUsed = ESP.getSketchSize();
  int flashUsage = (flashUsed * 100) / flashSize;

  // Temperature (approx)
  uint8_t temperature = temperatureRead();

  // Chip info
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);

  u8g2.drawStr(10, 10, "ESP32-S3 Status");

  char buf[32];

  // Box 1 - RAM
  u8g2.drawFrame(0, 12, 128, 12);
  sprintf(buf, "RAM: %d%% used", ramUsage);
  u8g2.drawStr(4, 21, buf);

  // Box 2 - Flash
  u8g2.drawFrame(0, 26, 128, 12);
  sprintf(buf, "Flash: %d%% used", flashUsage);
  u8g2.drawStr(4, 35, buf);

  // Box 3 - Temp
  u8g2.drawFrame(0, 40, 64, 12);
  sprintf(buf, "Temp: %d C", temperature);
  u8g2.drawStr(4, 49, buf);

  // Box 4 - Chip info
  u8g2.drawFrame(66, 40, 62, 12);
  sprintf(buf, "Cores: %d R:%d", chip_info.cores, chip_info.revision);
  u8g2.drawStr(68, 49, buf);

  u8g2.sendBuffer();

  delay(500);
}

