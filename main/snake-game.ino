// شبكة اللعبة
#define GRID_SIZE 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define GRID_COLS (SCREEN_WIDTH / GRID_SIZE)
#define GRID_ROWS (SCREEN_HEIGHT / GRID_SIZE)

enum Direction { UP, DOWN, LEFT, RIGHT };
Direction dir = RIGHT;

struct Point { int x, y; };

std::vector<Point> snake;
Point food;
unsigned long lastMoveTime = 0;
int moveDelay = 200;
bool gameOver = false;
int score = 0;
bool gameStarted = false;

void drawStartScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(25, 20, "SNAKE GAME");
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(15, 40, "Press SELECT to start");
  u8g2.sendBuffer();
}

void drawGameOverScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(20, 20, "GAME OVER");
  u8g2.setFont(u8g2_font_6x10_tr);
  char buf[20];
  sprintf(buf, "Score: %d", score);
  u8g2.drawStr(35, 35, buf);
  u8g2.drawStr(5, 55, "SELECT: Retry   BACK: Exit");
  u8g2.sendBuffer();
}

void placeFood() {
  while (true) {
    food.x = random(0, GRID_COLS);
    food.y = random(0, GRID_ROWS);
    bool onSnake = false;
    for (const auto& part : snake) {
      if (part.x == food.x && part.y == food.y) {
        onSnake = true;
        break;
      }
    }
    if (!onSnake) break;
  }
}

void snakeSetup() {

  randomSeed(millis());

  drawStartScreen();
}

void startGame() {
  snake.clear();
  snake.push_back({GRID_COLS / 2, GRID_ROWS / 2});
  dir = RIGHT;
  gameOver = false;
  score = 0;
  placeFood();
  lastMoveTime = millis();
  gameStarted = true;
}

void snakeLoop() {
  if (!gameStarted) {
    if (digitalRead(BTN_SELECT) == LOW) {
      delay(200);
      startGame();
    }
    return;
  }

  if (gameOver) {
    drawGameOverScreen();
    if (digitalRead(BTN_SELECT) == LOW) {
      delay(300);
      startGame();
    } else if (digitalRead(BTN_BACK) == LOW) {
      delay(300);
      gameStarted = false;
      drawStartScreen();
    }
    return;
  }

  // قراءة الأزرار
  if (digitalRead(BTN_UP) == LOW && dir != DOWN) dir = UP;
  else if (digitalRead(BTN_DOWN) == LOW && dir != UP) dir = DOWN;
  else if (digitalRead(BTN_LEFT) == LOW && dir != RIGHT) dir = LEFT;
  else if (digitalRead(BTN_RIGHT) == LOW && dir != LEFT) dir = RIGHT;

  if (millis() - lastMoveTime < moveDelay) return;
  lastMoveTime = millis();

  // تحديث موقع الرأس
  Point head = snake[0];
  switch (dir) {
    case UP: head.y--; break;
    case DOWN: head.y++; break;
    case LEFT: head.x--; break;
    case RIGHT: head.x++; break;
  }

  // التحقق من الاصطدام
  if (head.x < 0 || head.y < 0 || head.x >= GRID_COLS || head.y >= GRID_ROWS) {
    gameOver = true;
    return;
  }
  for (int i = 0; i < snake.size(); i++) {
    if (snake[i].x == head.x && snake[i].y == head.y) {
      gameOver = true;
      return;
    }
  }

  // تحديث الجسم
  snake.insert(snake.begin(), head);

  // أكل التفاحة
  if (head.x == food.x && head.y == food.y) {
    score++;
    placeFood();
  } else {
    snake.pop_back();
  }

  // رسم كل شيء
  u8g2.clearBuffer();

  // النقاط
  u8g2.setFont(u8g2_font_5x8_tr);
  char scoreStr[10];
  sprintf(scoreStr, "Score: %d", score);
  u8g2.drawStr(2, 8, scoreStr);

  // التفاحة (دائرة)
  u8g2.drawDisc(food.x * GRID_SIZE + GRID_SIZE / 2,
                food.y * GRID_SIZE + GRID_SIZE / 2,
                GRID_SIZE / 2);

  // جسم الثعبان
  for (int i = 1; i < snake.size(); i++) {
    u8g2.drawBox(snake[i].x * GRID_SIZE, snake[i].y * GRID_SIZE, GRID_SIZE, GRID_SIZE);
  }

  // رأس الثعبان (مربع مميز)
  u8g2.drawFrame(snake[0].x * GRID_SIZE, snake[0].y * GRID_SIZE, GRID_SIZE, GRID_SIZE);

  u8g2.sendBuffer();
}