
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


