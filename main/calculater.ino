String expression = "";
String result = "";

const char keyboard[3][5] = {
  {'7', '8', '9', '/', 'C'},
  {'4', '5', '6', '*', '.'},
  {'1', '2', '3', '-', '='},
};

const int rows = 3;
const int cols = 5;

int cursorRow = 0;
int cursorCol = 0;

void claculaterloop(){
  drawCalculatorUI();

  if (digitalRead(BTN_UP) == LOW) {
    moveCursor(-1, 0);
    delay(200);
  }
  if (digitalRead(BTN_DOWN) == LOW) {
    moveCursor(1, 0);
    delay(200);
  }
  if (digitalRead(BTN_LEFT) == LOW) {
    moveCursor(0, -1);
    delay(200);
  }
  if (digitalRead(BTN_RIGHT) == LOW) {
    moveCursor(0, 1);
    delay(200);
  }
  if (digitalRead(BTN_SELECT) == LOW) {
    handleSelect();
    delay(200);
  }
  if (digitalRead(BTN_BACK) == LOW) {
    if (expression.length() > 0) expression.remove(expression.length() - 1);
    delay(200);
  }
}

void drawCalculatorUI() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);

  // عرض المعادلة والنتيجة
  u8g2.setCursor(0, 10);
  u8g2.print("Expr: ");
  u8g2.print(expression);

  u8g2.setCursor(0, 20);
  u8g2.print("Res: ");
  u8g2.print(result);

  // أبعاد الأزرار
  int boxW = 20;
  int boxH = 12;
  int startX = 0;
  int startY = 24; // تم رفع الكيبورد عدة بيكسلات لأعلى
  int spacingX = 2;
  int spacingY = 2;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      int x = startX + c * (boxW + spacingX);
      int y = startY + r * (boxH + spacingY);
      char ch = keyboard[r][c];

      if (r == cursorRow && c == cursorCol) {
        u8g2.drawBox(x, y, boxW, boxH);
        u8g2.setDrawColor(0);
        u8g2.setCursor(x + 6, y + 9);
        u8g2.print(ch);
        u8g2.setDrawColor(1);
      } else {
        u8g2.drawFrame(x, y, boxW, boxH);
        u8g2.setCursor(x + 6, y + 9);
        u8g2.print(ch);
      }
    }
  }

  // زر 0 ثابت في الأعلى يمين
  u8g2.drawFrame(100, 10, 20, 12);
  u8g2.setCursor(106, 19);
  u8g2.print("0");

  u8g2.sendBuffer();
}

void moveCursor(int dr, int dc) {
  cursorRow = (cursorRow + dr + rows) % rows;
  cursorCol = (cursorCol + dc + cols) % cols;
}

void handleSelect() {
  char selected = keyboard[cursorRow][cursorCol];
  if (selected == 'C') {
    expression = "";
    result = "";
  } else if (selected == '=') {
    result = String(evalExpression(expression));
  } else {
    expression += selected;
  }
}

// تقييم المعادلة بشكل بسيط جداً (يدعم + و - فقط)
float evalExpression(String expr) {
  float total = 0;
  char op = '+';
  String num = "";
  for (int i = 0; i < expr.length(); i++) {
    char ch = expr[i];
    if (isdigit(ch) || ch == '.') {
      num += ch;
    } else {
      float val = num.toFloat();
      if (op == '+') total += val;
      else if (op == '-') total -= val;
      else if (op == '*') total *= val;
      else if (op == '/') total /= val;
      op = ch;
      num = "";
    }
  }
  if (num.length() > 0) {
    float val = num.toFloat();
    if (op == '+') total += val;
    else if (op == '-') total -= val;
    else if (op == '*') total *= val;
    else if (op == '/') total /= val;
  }
  return total;
}
