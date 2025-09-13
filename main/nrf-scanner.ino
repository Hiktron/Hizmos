
  // Constants
const uint8_t SCREEN_WIDTH = 128;
const uint8_t SCREEN_HEIGHT = 64;
const uint8_t GRAPH_HEIGHT = 54;
const uint8_t TOTAL_CHANNELS = 126;
const uint8_t SAMPLES = 4;

// Data arrays
uint8_t strength1[TOTAL_CHANNELS] = {0};
uint8_t strength2[TOTAL_CHANNELS] = {0};
uint16_t hits1[TOTAL_CHANNELS] = {0};
uint16_t hits2[TOTAL_CHANNELS] = {0};
uint8_t mostActive1 = 0;
uint8_t mostActive2 = 0;







void nrfscanner() {
  
  scanAll();
  drawnrfGraph();
  delay(120);
}



void scanAll() {
  for (uint8_t ch = 0; ch < TOTAL_CHANNELS; ch++) {
    uint8_t r1 = 0, r2 = 0;

    for (uint8_t i = 0; i < SAMPLES; i++) {
      radio1.setChannel(ch);
      delayMicroseconds(130);
      if (radio1.testRPD()) {
        r1++;
        hits1[ch]++;
      }

      radio2.setChannel(ch);
      delayMicroseconds(130);
      if (radio2.testRPD()) {
        r2++;
        hits2[ch]++;
      }
    }

    strength1[ch] = map(r1, 0, SAMPLES, 0, GRAPH_HEIGHT);
    strength2[ch] = map(r2, 0, SAMPLES, 0, GRAPH_HEIGHT);
  }

  uint16_t max1 = 0, max2 = 0;
  for (uint8_t ch = 0; ch < TOTAL_CHANNELS; ch++) {
    if (hits1[ch] > max1) {
      max1 = hits1[ch];
      mostActive1 = ch;
    }
    if (hits2[ch] > max2) {
      max2 = hits2[ch];
      mostActive2 = ch;
    }
  }
}

void drawnrfGraph() {
  u8g2.clearBuffer();

  // Header
  char label[32];
  sprintf(label, "R1 %d:%d | R2 %d:%d", mostActive1, hits1[mostActive1], mostActive2, hits2[mostActive2]);
  u8g2.drawStr(0, 8, label);

  float channelStep = 125.0 / (SCREEN_WIDTH - 1);  // Ensure last pixel maps to ch=125

  int prevY2 = -1;

  for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
    uint8_t ch = round(x * channelStep);
    if (ch >= TOTAL_CHANNELS) continue;

    uint8_t h1 = strength1[ch];
    uint8_t h2 = strength2[ch];

    // Smooth
    if (ch > 0) {
      h1 = (h1 + strength1[ch - 1]) / 2;
      h2 = (h2 + strength2[ch - 1]) / 2;
    }
    if (ch < TOTAL_CHANNELS - 1) {
      h1 = (h1 + strength1[ch + 1]) / 2;
      h2 = (h2 + strength2[ch + 1]) / 2;
    }

    uint8_t y1 = GRAPH_HEIGHT - h1;
    uint8_t y2 = GRAPH_HEIGHT - h2;

    // Radio 1 - filled vertical bar
    u8g2.drawVLine(x, y1, h1);

    // Radio 2 - mountain graph (not filled)
    if (prevY2 >= 0) {
      u8g2.drawLine(x - 1, prevY2, x, y2);
    }
    prevY2 = y2;
  }

  // Base line
  u8g2.drawLine(0, GRAPH_HEIGHT, SCREEN_WIDTH, GRAPH_HEIGHT);

  // Channel labels
  for (uint8_t ch = 0; ch <= 125; ch += 25) {
    uint8_t x = round((float)ch / 125 * (SCREEN_WIDTH - 1));
    sprintf(label, "%d", ch);
    u8g2.drawStr(x, GRAPH_HEIGHT + 8, label);
  }

  u8g2.sendBuffer();
}

