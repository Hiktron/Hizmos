
// Messages
const char* lines[] = {
  "Project by Hiktron",
  "Instagram: hiktron",
  "YouTube: hiktron",
  "Support us",
  "For more open",
  "source projects",
  "cyber boy",
};

const int totalLines = sizeof(lines) / sizeof(lines[0]);
const int visibleLines = 3;
int topLine = 0;




void drawScrollBar() {
  int barHeight = 64;
  int handleHeight = barHeight * visibleLines / totalLines;
  if (handleHeight < 4) handleHeight = 4;

  int handleY = (barHeight - handleHeight) * topLine / (totalLines - visibleLines);

  u8g2.drawFrame(123, 0, 4, barHeight);             // Scrollbar track
  u8g2.drawBox(124, handleY, 2, handleHeight);      // Scrollbar handle
}

void about() {
  if (digitalRead(BTN_DOWN) == LOW) {
    if (topLine < totalLines - visibleLines) topLine++;
    delay(200);
  }

  if (digitalRead(BTN_UP) == LOW) {
    if (topLine > 0) topLine--;
    delay(200);
  }

  // Draw screen
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);

  // Big rounded box to hold 3 lines
  u8g2.drawRFrame(0, 8, 122, 48, 8); // x, y, w, h, radius

  // Draw the 3 visible lines inside the big box
  for (int i = 0; i < visibleLines; i++) {
    int index = topLine + i;
    if (index < totalLines) {
      u8g2.setCursor(10, 22 + i * 14); // adjust spacing
      u8g2.print(lines[index]);
    }
  }

  drawScrollBar(); // Draw scroll indicator

  u8g2.sendBuffer();
}
