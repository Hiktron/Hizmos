
int focusDuration = 25;
int breakDuration = 5;
bool onBreak = false;
bool isRunning = false;
int currentDuration = 0;
unsigned long lastUpdate = 0;
int selectedSetting = 0;

void pomdorotimerloop(){
  if (isRunning && millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    if (currentDuration > 0) {
      currentDuration--;
    } else {
      onBreak = !onBreak;
      updateTimer();
    }
  }

  if (buttonPressed(BTN_LEFT)) selectedSetting = 0;
  if (buttonPressed(BTN_RIGHT)) selectedSetting = 1;

  if (buttonPressed(BTN_UP)) {
    if (selectedSetting == 0 && focusDuration < 60) focusDuration++;
    if (selectedSetting == 1 && breakDuration < 30) breakDuration++;
  }

  if (buttonPressed(BTN_DOWN)) {
    if (selectedSetting == 0 && focusDuration > 1) focusDuration--;
    if (selectedSetting == 1 && breakDuration > 1) breakDuration--;
  }

  if (buttonPressed(BTN_SELECT)) {
    isRunning = !isRunning;
    if (isRunning && currentDuration == 0) updateTimer();
  }

  if (buttonPressed(BTN_BACK)) {
    isRunning = false;
    updateTimer();
  }

  drawUI();
  delay(50);
}







bool buttonPressed(int pin) {
  return digitalRead(pin) == LOW;
}

void updateTimer() {
  currentDuration = (onBreak ? breakDuration : focusDuration) * 60;
  lastUpdate = millis();
}

void drawUI() {
  u8g2.clearBuffer();

  //  الحالة: FOCUS أو BREAK
  u8g2.setFont(u8g2_font_6x12_tr);
  String modeStr = isRunning ? (onBreak ? "BREAK " : "FOCUS ") : "POMODORO TIMER";
  int w = u8g2.getStrWidth(modeStr.c_str());
  u8g2.setCursor((128 - w) / 2, 10);
  u8g2.print(modeStr);

  //  إعدادات Focus و Break
  u8g2.setFont(u8g2_font_5x8_tr);
  u8g2.setCursor(5, 22);
  u8g2.print("Focus:");
  u8g2.setCursor(40, 22);
  u8g2.print(focusDuration);
  u8g2.print("m");

  u8g2.setCursor(75, 22);
  u8g2.print("Break:");
  u8g2.setCursor(110, 22);
  u8g2.print(breakDuration);
  u8g2.print("m");

  //  المؤقت (داخل مستطيل دائري غير مملوء)
  int minutes = currentDuration / 60;
  int seconds = currentDuration % 60;
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", minutes, seconds);

  u8g2.drawRFrame(34, 27, 60, 24, 6); // إطار الوقت
  u8g2.setFont(u8g2_font_logisoso20_tr);
  int tw = u8g2.getStrWidth(timeStr);
  u8g2.setCursor((128 - tw) / 2, 47);
  u8g2.print(timeStr);

  //  أزرار التحكم: Start/Stop و Reset
  u8g2.setFont(u8g2_font_5x8_tr);

  // Start/Stop
  u8g2.drawBox(5, 54, 60, 10);
  u8g2.setDrawColor(0);
  u8g2.setCursor(10, 62);
  u8g2.print("[Start/Stop]");
  u8g2.setDrawColor(1);

  // Reset
  u8g2.drawBox(68, 54, 55, 10);
  u8g2.setDrawColor(0);
  u8g2.setCursor(76, 62);
  u8g2.print("[ Reset ]");
  u8g2.setDrawColor(1);

  u8g2.sendBuffer();
}