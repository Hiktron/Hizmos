void restartesp() {
  // لو SELECT مضغوط → إعادة تشغيل
  if (digitalRead(BTN_SELECT) == LOW) {
    delay(200);
    restartDevice();
  }

  // لو BACK مضغوط → إلغاء
  if (digitalRead(BTN_BACK) == LOW) {
    delay(200);
    drawCancelledMessage();
    while (1); // توقف البرنامج
  }
}

void drawConfirmationUI() {
  u8g2.clearBuffer();

  // إطار رئيسي
  u8g2.drawRFrame(0, 0, 128, 64, 4); // إطار دائري خفيف

  // نص الرسالة
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(20, 20, "Are you sure you");
  u8g2.drawStr(20, 34, "want to restart?");

  // زر YES
  u8g2.drawBox(10, 45, 45, 15); // خلفية زر YES
  u8g2.setDrawColor(0); // لون النص داخل الزر
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(20, 56, "YES");
  u8g2.setDrawColor(1); // إعادة اللون الطبيعي

  // زر NO
  u8g2.drawFrame(70, 45, 45, 15); // إطار زر NO
  u8g2.drawStr(85, 56, "NO");

  u8g2.sendBuffer();
}

void drawCancelledMessage() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(30, 30, "Cancelled!");
  u8g2.sendBuffer();
}

void restartDevice() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(30, 30, "Restarting...");
  u8g2.sendBuffer();
  delay(1000);
  ESP.restart();
}
