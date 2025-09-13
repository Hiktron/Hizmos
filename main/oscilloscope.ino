

void handleoscillomenu() {
  static const char* oscilloMenuItems[] = {"Analog Read", "Wave Creator"};
  static int oscilloMenuIndex = 0;
  int menuLength = sizeof(oscilloMenuItems) / sizeof(oscilloMenuItems[0]);

  // Handle input
  if (!digitalRead(BTN_UP)) {
    oscilloMenuIndex--;
    if (oscilloMenuIndex < 0) oscilloMenuIndex = menuLength - 1;
    delay(150);
  } else if (!digitalRead(BTN_DOWN)) {
    oscilloMenuIndex++;
    if (oscilloMenuIndex >= menuLength) oscilloMenuIndex = 0;
    delay(150);
  }

  // Display menu
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(10, 10, "Oscilloscope Menu");

  for (int i = 0; i < menuLength; i++) {
    if (i == oscilloMenuIndex) {
      u8g2.drawBox(8, 18 + i * 12, 112, 12);
      u8g2.setDrawColor(0);
      u8g2.drawStr(14, 27 + i * 12, oscilloMenuItems[i]);
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(14, 27 + i * 12, oscilloMenuItems[i]);
    }
  }

  u8g2.sendBuffer();

  // Selection
  if (!digitalRead(BTN_SELECT)) {
    delay(150);
    if (oscilloMenuIndex == 0) {
      runLoop(analogread);     // Shows analog signal
    } else if (oscilloMenuIndex == 1) {
      runLoop(wavecreator);    // Opens waveform drawer
    }
  }
}



const char* waveTypes[] = {"Sine", "Square", "Triangle", "Sawtooth", "Pulse", "Noise"};
int waveCount = sizeof(waveTypes) / sizeof(waveTypes[0]);

int selectedWave = 0;
int frequency = 10;
bool inMenu = true;
int offset = 0;




void drawSwipeMenu() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr); // خط أصغر
  u8g2.drawStr(34, 10, "Choose Wave");

  int centerX = 64;
  int spacing = 42;

  for (int i = -1; i <= 1; i++) {
    int index = (selectedWave + i + waveCount) % waveCount;
    int x = centerX + i * spacing;
    if (i == 0) {
      // Selected
      u8g2.drawRBox(x - 24, 26, 48, 18, 3);
      u8g2.setDrawColor(0);
      u8g2.drawStr(x - strlen(waveTypes[index]) * 3, 38, waveTypes[index]);
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(x - strlen(waveTypes[index]) * 3, 38, waveTypes[index]);
    }
  }

  // arrows
  u8g2.drawStr(0, 38, "<");
  u8g2.drawStr(122, 38, ">");
  u8g2.sendBuffer();
}

void drawWaveform(const char* type, int frequency, int offset) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr); // خط أصغر
  u8g2.drawStr(0, 10, type);

  int height = 40;
  int midY = 32;
  int period = max(1, 128 / frequency);
  int prevY = midY;

  for (int x = 0; x < 128; x++) {
    int trueX = (x + offset) % period;
    int y = midY;

    if (strcmp(type, "Sine") == 0) {
      float rad = 2 * PI * trueX / period;
      y = midY + (int)(-sin(rad) * height / 2);
    }
    else if (strcmp(type, "Square") == 0) {
      y = (trueX < period / 2) ? (midY - height / 2) : (midY + height / 2);
    }
    else if (strcmp(type, "Triangle") == 0) {
      int half = period / 2;
      if (trueX < half)
        y = map(trueX, 0, half, midY + height / 2, midY - height / 2);
      else
        y = map(trueX, half, period, midY - height / 2, midY + height / 2);
    }
    else if (strcmp(type, "Sawtooth") == 0) {
      y = map(trueX, 0, period, midY + height / 2, midY - height / 2);
    }
    else if (strcmp(type, "Pulse") == 0) {
      y = (trueX < 3) ? (midY - height / 2) : (midY + height / 2);
    }
    else if (strcmp(type, "Noise") == 0) {
      y = random(midY - height / 2, midY + height / 2);
    }

    if (x > 0) u8g2.drawLine(x - 1, prevY, x, y);
    prevY = y;
  }

  u8g2.sendBuffer();
}



void wavecreator() {
  if (inMenu) {
    drawSwipeMenu();

    if (!digitalRead(BTN_LEFT)) {
      selectedWave = (selectedWave - 1 + waveCount) % waveCount;
      delay(50);
    } else if (!digitalRead(BTN_RIGHT)) {
      selectedWave = (selectedWave + 1) % waveCount;
      delay(50);
    } else if (!digitalRead(BTN_SELECT)) {
      inMenu = false;
      delay(50);
    }
  } else {
    drawWaveform(waveTypes[selectedWave], frequency, offset);
    offset += 1;
    delay(40);

    if (!digitalRead(BTN_UP) && frequency < 100) {
      frequency++;
      delay(50);
    } else if (!digitalRead(BTN_DOWN) && frequency > 1) {
      frequency--;
      delay(50);
    } else if (!digitalRead(BTN_BACK)) {
      inMenu = true;
      delay(150);
    }
  }
}













///////////the analog read//////////////////

bool analogread_warningShown = false;
bool analogread_helpShown = false;
bool analogread_frozen = false;
bool analogread_showHUD = false;
int analogread_zoomLevel = 1;  // 1 = default


void analogread() {
  static int values[60];
  static int frozenValues[60];
  const int graphTop = 12;
  const int graphBottom = 58;

  // === Safety warning (once) ===
  if (!analogread_warningShown) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(5, 20, "WARNING!");
    u8g2.drawStr(5, 35, "Do NOT input voltage");
    u8g2.drawStr(5, 45, "over 3.3V to this pin!");
    u8g2.sendBuffer();
    delay(3000);
    analogread_warningShown = true;
    return;
  }

  // === Help screen (once) ===
  if (!analogread_helpShown) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawStr(5, 10, "BTN_SELECT: Freeze/Unfreeze");
    u8g2.drawStr(5, 18, "BTN_RIGHT : Zoom In");
    u8g2.drawStr(5, 26, "BTN_LEFT  : Zoom Out");
    u8g2.drawStr(5, 34, "BTN_DOWN  : HUD (Vpp, Avg, Freq)");
    u8g2.drawStr(5, 42, "BTN_UP    : Return to Graph");
    u8g2.sendBuffer();
    delay(3500);
    analogread_helpShown = true;
    return;
  }

  // === Input Handling ===
  if (!digitalRead(BTN_SELECT)) {
    analogread_frozen = !analogread_frozen;
    delay(200);
  }
  if (!digitalRead(BTN_LEFT)) {
    analogread_zoomLevel = max(1, analogread_zoomLevel - 1);
    delay(100);
  }
  if (!digitalRead(BTN_RIGHT)) {
    analogread_zoomLevel = min(5, analogread_zoomLevel + 1);
    delay(100);
  }
  if (!digitalRead(BTN_DOWN)) {
    analogread_showHUD = true;
    delay(200);
  }
  if (!digitalRead(BTN_UP)) {
    analogread_showHUD = false;
    delay(200);
  }

  // === Read Value ===
  if (!analogread_frozen && !analogread_showHUD) {
    for (int i = 0; i < 59; i++) values[i] = values[i + 1];
    int raw = analogRead(ANALOG_PIN);
    values[59] = raw;
  }

  int* displayData = analogread_frozen ? frozenValues : values;
  if (analogread_frozen) memcpy(frozenValues, values, sizeof(values));

  int raw = displayData[59];
  float voltage = raw * (3.3 / 4095.0);

  // === Auto Zoom-Out if clipped ===
  int maxDisplayValue = 4095 / analogread_zoomLevel;
  if (raw > maxDisplayValue && analogread_zoomLevel > 1) {
    analogread_zoomLevel--;
  }

  // === Clear Screen ===
  u8g2.clearBuffer();

  // === Top Horizontal Voltage Bar ===
  int barLength = map(raw, 0, 4095, 0, 128);
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 128, 10);
  u8g2.setDrawColor(1);
  u8g2.drawBox(0, 0, barLength, 10);

  // === HUD Mode ===
  if (analogread_showHUD) {
    // Compute stats
    int minVal = 4095, maxVal = 0, sum = 0;
    for (int i = 0; i < 60; i++) {
      int val = displayData[i];
      minVal = min(minVal, val);
      maxVal = max(maxVal, val);
      sum += val;
    }
    float vpp = (maxVal - minVal) * 3.3 / 4095.0;
    float avg = (sum / 60.0) * 3.3 / 4095.0;
    int freq = 412;  // Dummy

    // Draw HUD
    u8g2.setFont(u8g2_font_6x10_tr);
    char line1[20], line2[20], line3[20];
    sprintf(line1, "Vpp: %.2f V", vpp);
    sprintf(line2, "Avg: %.2f V", avg);
    sprintf(line3, "Freq: %d Hz", freq);

    u8g2.drawStr(10, 25, line1);
    u8g2.drawStr(10, 40, line2);
    u8g2.drawStr(10, 55, line3);

    u8g2.sendBuffer();
    delay(100);
    return;
  }

  // === Right Side Vertical Ruler ===
  u8g2.setFont(u8g2_font_4x6_tr);
  for (int i = 0; i <= 4; i++) {
    int y = map(i, 0, 4, graphTop, graphBottom);
    float level = (3.3 / analogread_zoomLevel) * i / 4.0;
    char ruler[6];
    dtostrf(level, 2, 1, ruler);
    u8g2.drawStr(115, y, ruler);
    u8g2.drawHLine(110, y, 5);  // tick
  }

  // === Draw waveform ===
  for (int x = 1; x < 60; x++) {
    int y1 = map(displayData[x - 1], 0, maxDisplayValue, graphBottom, graphTop);
    int y2 = map(displayData[x],     0, maxDisplayValue, graphBottom, graphTop);
    y1 = constrain(y1, graphTop, graphBottom);
    y2 = constrain(y2, graphTop, graphBottom);
    u8g2.drawLine(x + 2, y1, x + 3, y2);
  }

  // === Voltage Text ===
  char buf[10];
  dtostrf(voltage, 4, 2, buf);
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(2, 64, "Volt:");
  u8g2.drawStr(30, 64, buf);
  u8g2.drawStr(60, 64, "V");

  // === Final Draw ===
  u8g2.sendBuffer();
  delay(20);
}
