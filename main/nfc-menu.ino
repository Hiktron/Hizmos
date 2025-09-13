void handlenfcmenu() {
  static bool initialized = false;

  if (!initialized) {
    Wire.begin();          // Use default SDA/SCL or specify custom pins
    nfc.begin();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
      Serial.println("PN532 not found");
      return;
    }

    nfc.SAMConfig();       // NFC module ready
    initialized = true;
  }

  uint8_t uid[7];    // Buffer to store the returned UID
  uint8_t uidLength;

  Serial.println("Waiting for an NFC tag...");

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) Serial.print("0");
      Serial.print(uid[i], HEX);
    }
    Serial.println();
  } else {
    Serial.println("No tag detected.");
  }
}
