// ==== Format State ====
enum FormaterState {
  FORMAT_ASK,
  FORMAT_CONFIRM,
  FORMAT_PROGRESS,
  FORMAT_DONE
};

FormaterState formaterState = FORMAT_ASK;
bool formaterLastSelectState = HIGH;

// ==== رسم رسالة داخل بوكس دائري ====
void drawBoxMessage(const char* line1, const char* line2 = nullptr, const char* footer = nullptr) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);

  u8g2.drawRBox(6, 14, 116, 36, 6);  // Box
  u8g2.setDrawColor(0);

  if (line1) {
    int w1 = u8g2.getStrWidth(line1);
    u8g2.setCursor((128 - w1) / 2, 28);
    u8g2.print(line1);
  }

  if (line2) {
    int w2 = u8g2.getStrWidth(line2);
    u8g2.setCursor((128 - w2) / 2, 40);
    u8g2.print(line2);
  }

  u8g2.setDrawColor(1);

  if (footer) {
    int wf = u8g2.getStrWidth(footer);
    u8g2.setCursor((128 - wf) / 2, 62);
    u8g2.print(footer);
  }

  u8g2.sendBuffer();
}

// ==== حذف كل الملفات ====
void formaterDeleteAllFromSD(File dir) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;

    if (entry.isDirectory()) {
      formaterDeleteAllFromSD(entry);
      SD.rmdir(entry.name());
    } else {
      SD.remove(entry.name());
    }

    entry.close();
  }
  dir.close();
}

// ==== شريط التحميل ====
void formaterDrawProgress(int progress) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(34, 15, "Formatting...");
  u8g2.drawFrame(10, 30, 108, 12);
  u8g2.drawBox(10, 30, progress, 12);
  u8g2.sendBuffer();
}

// ==== تنفيذ عملية الفورمات ====
void formaterStart() {
  for (int p = 0; p <= 108; p += 10) {
    formaterDrawProgress(p);
    delay(100);
  }

  File root = SD.open("/");
  formaterDeleteAllFromSD(root);
  delay(300);

  formaterState = FORMAT_DONE;
  drawBoxMessage("SD Card", "Formatted!", "Returning...");
  delay(2000);

  formaterState = FORMAT_ASK;
  drawBoxMessage("Do you want to", "format SD card?", "Press SELECT to continue");
}



void sdformater() {
  bool selectPressed = digitalRead(BTN_SELECT) == LOW;
  bool backPressed = digitalRead(BTN_BACK) == LOW;

  if (selectPressed && formaterLastSelectState == HIGH) {
    switch (formaterState) {
      case FORMAT_ASK:
        formaterState = FORMAT_CONFIRM;
        drawBoxMessage("Are you sure?", "Data will be erased!", "SELECT=Yes  BACK=No");
        break;

      case FORMAT_CONFIRM:
        formaterState = FORMAT_PROGRESS;
        formaterStart();
        break;

      default:
        break;
    }
  }

  if (backPressed && formaterState == FORMAT_CONFIRM) {
    formaterState = FORMAT_ASK;
    drawBoxMessage("Do you want to", "format SD card?", "Press SELECT to continue");
  }

  formaterLastSelectState = !selectPressed;
  delay(100);
}
