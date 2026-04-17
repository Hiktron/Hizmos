uint8_t i2cFoundAddresses[32];
uint8_t i2cDeviceCount = 0;
uint8_t i2cSelectedIndex = 0;

void setupi2c(){

 Wire.begin(I2C_SDA, I2C_SCL);
 i2cScan();
}



void i2cscanner() {
  if (digitalRead(BTN_UP) == LOW) {
    i2cSelectedIndex = (i2cSelectedIndex == 0) ? i2cDeviceCount - 1 : i2cSelectedIndex - 1;
    delay(200);
  }
  if (digitalRead(BTN_DOWN) == LOW) {
    i2cSelectedIndex = (i2cSelectedIndex + 1) % i2cDeviceCount;
    delay(200);
  }

  i2cDrawMenu();

  if (digitalRead(BTN_SELECT) == LOW) {
    i2cShowDeviceInfo(i2cFoundAddresses[i2cSelectedIndex]);
    delay(500);
  }
}

void i2cDrawMenu() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);

  uint8_t itemsPerPage = 5;
  uint8_t start = (i2cSelectedIndex / itemsPerPage) * itemsPerPage;

  for (uint8_t i = 0; i < itemsPerPage && (start + i) < i2cDeviceCount; i++) {
    uint8_t actualIndex = start + i;
    int y = i * 13;

    u8g2.drawFrame(0, y, 128, 13);

    char line[32];
    sprintf(line, "0x%02X  %s", i2cFoundAddresses[actualIndex], identifyDevice(i2cFoundAddresses[actualIndex]));

    if (actualIndex == i2cSelectedIndex) {
      u8g2.drawRBox(3, y + 2, 122, 9, 3);
      u8g2.setDrawColor(0);
      u8g2.setCursor(6, y + 10);
      u8g2.print(line);
      u8g2.setDrawColor(1);
    } else {
      u8g2.setCursor(6, y + 10);
      u8g2.print(line);
    }
  }

  u8g2.sendBuffer();
}

void i2cScan() {
  i2cDeviceCount = 0;
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      i2cFoundAddresses[i2cDeviceCount++] = address;
      delay(5);
    }
  }
}

void i2cShowDeviceInfo(uint8_t address) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.setCursor(0, 12);
  u8g2.print("Address: 0x");
  u8g2.print(address, HEX);
  u8g2.setCursor(0, 28);
  u8g2.print("Device:");
  u8g2.setCursor(0, 44);
  u8g2.print(identifyDevice(address));
  u8g2.sendBuffer();
  delay(1500);
}

const char* identifyDevice(uint8_t address) {
  switch (address) {
    case 0x0C: return "FXOS8700";
    case 0x0D: return "FXAS21002";
    case 0x13: return "VCNL4020";
    case 0x1D: return "ADXL345";
    case 0x1E: return "HMC5883L";
    case 0x20: case 0x21: case 0x22: return "PCF8574 I/O Expander";
    case 0x23: return "PCF8574 or BH1750";
    case 0x24: return "pn532 NFC Tag or PCF8574";
    case 0x25: case 0x26: case 0x27: return "PCF8574 I/O Expander";
    case 0x29: return "APDS9960 / VL53L0X / TCS34725";
    case 0x2A: return "MMA8452Q";
    case 0x30: return "VL6180X ToF Sensor";
    case 0x33: return "MLX90640 IR Array";
    case 0x38: return "MCP23017 / FT6236";
    case 0x39: return "TSL2561 Light Sensor";
    case 0x3B: return "Grove OLED Display";
    case 0x3C: return "SSD1306 OLED Display";
    case 0x3D: return "SH1106 OLED Display";
    case 0x3E: return "PN532 NFC Reader (Alt Addr)";
    case 0x40: return "Si7021 / HTU21D / PCA9685";
    case 0x44: case 0x45: return "SHT3x Temp & Humidity";
    case 0x48: return "ADS1115 / TMP102";
    case 0x49: return "ADS1015 / TMP112 / TSL2591";
    case 0x50: return "24C32 EEPROM";
    case 0x51: return "24C64 EEPROM";
    case 0x52: return "24C128 / ADS1115 / VCNL4010 / PN532 NFC";
    case 0x53: return "24C256 EEPROM";
    case 0x54: case 0x55: case 0x56: case 0x57: return "AT24Cxx EEPROM";
    case 0x58: return "CCS811 Air Quality";
    case 0x5A: return "MLX90614 IR Temp / MPR121 Touch";
    case 0x5B: return "SGP30 Gas Sensor";
    case 0x60: return "MCP4725 DAC";
    case 0x61: return "SCD30 CO2 / MCP4726 DAC";
    case 0x66: return "INA226 Power Monitor";
    case 0x68: return "DS1307/DS3231/MPU6050";
    case 0x69: return "MPU9250 / MPU6500";
    case 0x6A: return "LSM6DS3 IMU";
    case 0x6B: return "MPU6000 IMU";
    case 0x6D: return "LPS22HB Pressure Sensor";
    case 0x70: return "TCA9548A I2C Switch (0x70)";
    case 0x71: return "TCA9548A I2C Switch (0x71)";
    case 0x72: return "TCA9548A I2C Switch (0x72)";
    case 0x73: return "TCA9548A I2C Switch (0x73)";
    case 0x74: return "TCA9548A I2C Switch (0x74)";
    case 0x75: return "TCA9548A I2C Switch (0x75)";
    case 0x76: return "BME280 / BMP280";
    case 0x77: return "BMP180 / BMP085 / BME280";
    default: return "Unknown Device";
  }
}

