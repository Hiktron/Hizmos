// Player settings
int invadersPlayerX = 50;
int invadersPlayerY = 56;
bool invadersPlayerAlive = true;

// Bullet settings
#define INVADERS_MAX_BULLETS 6
struct Bullet {
  int x, y;
  bool active;
};
Bullet invadersBullets[INVADERS_MAX_BULLETS];

// Enemy settings
#define INVADERS_MAX_ENEMIES 4
struct Enemy {
  int x, y;
  bool active;
};
Enemy invadersEnemies[INVADERS_MAX_ENEMIES];

// Explosion
bool invadersExploding = false;
unsigned long invadersExplosionStart = 0;

// Score
int invadersScore = 0;

// Timing
unsigned long invadersLastEnemyMove = 0;
unsigned long invadersLastBulletMove = 0;
unsigned long invadersLastEnemySpawn = 0;

// Player shape (bigger, horizontal ship)
// شكل الـ Fighter Jet (11x9)
const uint8_t invadersPlayerShape[9] = {
  0b00011000,     
  0b01111110,    
  0b00011000,   
  0b10011001,  
  0b11111111,  
  0b00111100,     
  0b00111100,    
  0b01111110,   
  0b11111111    
};


// Rock shape (enemy)
const uint8_t invadersRockShape[5] = {
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00100
};

void invadersDrawPlayer() {
  for (int y = 0; y < 7; y++) {
    for (int x = 0; x < 8; x++) {
      if (bitRead(invadersPlayerShape[y], 7 - x)) {
        u8g2.drawPixel(invadersPlayerX + x, invadersPlayerY + y);
      }
    }
  }
}

void invadersDrawRock(int x, int y) {
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < 5; i++) {
      if (bitRead(invadersRockShape[j], 4 - i)) {
        u8g2.drawPixel(x + i, y + j);
      }
    }
  }
}

void invadersShoot() {
  for (int i = 0; i < INVADERS_MAX_BULLETS; i++) {
    if (!invadersBullets[i].active) {
      invadersBullets[i].x = invadersPlayerX + 3;
      invadersBullets[i].y = invadersPlayerY - 2;
      invadersBullets[i].active = true;
      break;
    }
  }
}

void invadersMoveBullets() {
  for (int i = 0; i < INVADERS_MAX_BULLETS; i++) {
    if (invadersBullets[i].active) {
      invadersBullets[i].y -= 7;
      if (invadersBullets[i].y < 0) {
        invadersBullets[i].active = false;
      }
    }
  }
}

void invadersMoveEnemies() {
  for (int i = 0; i < INVADERS_MAX_ENEMIES; i++) {
    if (invadersEnemies[i].active) {
      invadersEnemies[i].y += 2;

      if (invadersEnemies[i].y + 5 >= invadersPlayerY &&
          invadersEnemies[i].x + 4 >= invadersPlayerX &&
          invadersEnemies[i].x <= invadersPlayerX + 7) {
        invadersEnemies[i].active = false;
        invadersPlayerAlive = false;
        invadersExploding = true;
        invadersExplosionStart = millis();
      } else if (invadersEnemies[i].y > 64) {
        invadersEnemies[i].active = false;
      }
    }
  }
}

void invadersSpawnEnemy() {
  for (int i = 0; i < INVADERS_MAX_ENEMIES; i++) {
    if (!invadersEnemies[i].active) {
      invadersEnemies[i].x = random(0, 120);
      invadersEnemies[i].y = 0;
      invadersEnemies[i].active = true;
      break;
    }
  }
}

void invadersCheckCollisions() {
  for (int i = 0; i < INVADERS_MAX_BULLETS; i++) {
    if (!invadersBullets[i].active) continue;

    for (int j = 0; j < INVADERS_MAX_ENEMIES; j++) {
      if (!invadersEnemies[j].active) continue;

      if (invadersBullets[i].x >= invadersEnemies[j].x &&
          invadersBullets[i].x <= invadersEnemies[j].x + 4 &&
          invadersBullets[i].y >= invadersEnemies[j].y &&
          invadersBullets[i].y <= invadersEnemies[j].y + 4) {
        invadersBullets[i].active = false;
        invadersEnemies[j].active = false;
        invadersScore++;
      }
    }
  }
}

void invadersDrawExplosion() {
  int cx = invadersPlayerX + 4;
  int cy = invadersPlayerY + 3;
  for (int r = 1; r < 6; r++) {
    u8g2.drawCircle(cx, cy, r);
  }
}

void invadersDrawGame() {
  u8g2.clearBuffer();

  // Enemies
  for (int i = 0; i < INVADERS_MAX_ENEMIES; i++) {
    if (invadersEnemies[i].active) {
      invadersDrawRock(invadersEnemies[i].x, invadersEnemies[i].y);
    }
  }

  // Bullets (as small squares)
  for (int i = 0; i < INVADERS_MAX_BULLETS; i++) {
    if (invadersBullets[i].active) {
      u8g2.drawBox(invadersBullets[i].x, invadersBullets[i].y, 2, 2);
    }
  }

  // Player or explosion
  if (invadersPlayerAlive) {
    invadersDrawPlayer();
  } else if (invadersExploding) {
    invadersDrawExplosion();
  }

  // Score
  u8g2.setFont(u8g2_font_5x8_tf);
  u8g2.setCursor(0, 10);
  u8g2.print("Score:");
  u8g2.print(invadersScore);

  u8g2.sendBuffer();
}








void spacegame() {
  unsigned long now = millis();

  // Player controls
  if (invadersPlayerAlive) {
    if (!digitalRead(BTN_LEFT) && invadersPlayerX > 0) {
      invadersPlayerX -= 7;
    }
    if (!digitalRead(BTN_RIGHT) && invadersPlayerX < 117) {
      invadersPlayerX += 7;
    }
    if (!digitalRead(BTN_SELECT)) {
      invadersShoot();
      delay(50);  // Fire rate limit
    }
  } else if (invadersExploding && (now - invadersExplosionStart > 900)) {
    // Restart
    invadersPlayerAlive = true;
    invadersExploding = false;
    invadersScore = 0;
    for (int i = 0; i < INVADERS_MAX_ENEMIES; i++) invadersEnemies[i].active = false;
    for (int i = 0; i < INVADERS_MAX_BULLETS; i++) invadersBullets[i].active = false;
    invadersPlayerX = 50;
  }

  // Game logic
  if (now - invadersLastBulletMove > 40) {
    invadersMoveBullets();
    invadersLastBulletMove = now;
  }

  if (now - invadersLastEnemyMove > 50) {
    invadersMoveEnemies();
    invadersLastEnemyMove = now;
  }

  if (now - invadersLastEnemySpawn > 900 && invadersPlayerAlive) {
    invadersSpawnEnemy();
    invadersLastEnemySpawn = now;
  }

  invadersCheckCollisions();
  invadersDrawGame();
}



