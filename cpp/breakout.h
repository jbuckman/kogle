#pragma once
#include "game.h"
#include <math.h>

class Breakout : public Game {
public:
  Breakout() {
    resetBallAndPaddle();
  }
  std::vector<bool> legalActions() override {
    //      NOOP,  FIRE, UP,   DOWN,   LEFT, RIGHT
    return {true,   false, false, false, true, true};
  }
  void renderPixels(uint8_t * buffer) override {
    memset(buffer, 0, sizeof(uint8_t) * 64 * 64);

    paddle.render(buffer);
    ball.render(buffer);

    float y = rowStart;
    int colorIndex = 0;
    for (const auto& row : rows) {
      const int color = boringColors[colorIndex % 6];
      colorIndex++;
      constexpr int rowSize = sizeof(row) / sizeof(row[0]);
      // Draw the entire row and clear it later.
      // This is faster when the row is mostly full.
      // Could be even faster by checking how full the row is
      // but I am lazy.
      int start = 64 * floor(64.0 * y);
      int end = 64 * floor(64.0 * y + 64.0 * rowHeight);
      memset(&buffer[start], color, end - start);
      for (int x = 0; x < rowSize; x++) {
        if (!row[x])
          drawRectDangerously(buffer, 64.0 * x / rowSize, 64.0 * y, 64.0 / rowSize, 64.0 * rowHeight, 0);
      }
      y += rowHeight;
    }
    drawNumber(buffer, 1, 1, score);
  }
  std::vector<int> step(uint8_t action) override {
    bool Up = (action == 2);
    bool Down = (action == 3);
    bool Left = (action == 4);
    bool Right = (action == 5); 

    float lastScore = this->score;
    //ball.x += ball.vx;
    //ball.y += ball.vy;
    // // Auto Player
    // if (abs(paddle.x - ball.x) > paddleSpeed) {
    //   if (paddle.x < ball.x) {
    //     paddle.x += paddleSpeed;
    //   } else {
    //     paddle.x -= paddleSpeed;
    //   }
    // }

    float humanSpeed = 0;
    if (Right)
      paddle.setX(paddle.getX()+0.5);
    if (Left)
      paddle.setX(paddle.getX()-0.5);
 
    if (paddle.collide(ball)) {
      doVolley();
    }
    float y = rowStart;
    for (int i = 0; i < sizeof(rows) / sizeof(rows[0]); i++) {
       auto& row = rows[i];
       for (int x = 0; x < sizeof(row) / sizeof(row[0]); x++) {
        if (!row[x])
          continue;
        if (ball.collide((x / 16.0f + 0.5f / 16.0f)*64.0, (y + rowHeight / 2.0f)*64.0f, 1.0 / 16.0f*64.0f, rowHeight*64.0f)) {
          row[x] = 0;
          ball.setVy(-ball.getVy());
          score += scores[i];
          goto outer;
        }
      }
      y += rowHeight;
    }
    outer:;
    if (ball.getY() < 0) {
      ball.setY(0);
      ball.setVy(-ball.getVy());
    }
    if (ball.getX() > 63) {
       ball.setX(63);
       ball.setVx(-ball.getVx());
    }
    if (ball.getX() < 0) {
      ball.setX(0);
      ball.setVx(-ball.getVx());
    }
    paddle.setX(std::max(0.0f, std::min(63.0f, paddle.getX())));
    ball.update();
    const bool isDone = ball.getY() > 63.0;
    const int score = this->score - lastScore;
    if (isDone)
      resetBallAndPaddle();
    return {score, isDone};
  }
private:

  static constexpr float slowSpeed = 0.35;
  static constexpr float paddleSpeed = 0.5 / 64.0;
  static constexpr float paddleSize = 10.0 / 64.0;
  static constexpr float ballSize = 2.0 / 64.0;
  static constexpr float rowHeight = 2.0 / 64.0;
  static constexpr float rowStart = 20.0 / 64.0;
  static constexpr int scores[] = {7, 7, 4, 4, 1, 1};

  static constexpr int boringColors[] = {75, 100, 150, 200, 225, 250};
  const bool numberPatterns[10][5][3] = {{
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
  }, {
    {0, 1, 0},
    {1, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
  }, {
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
  }, {
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
  }, {
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {0, 0, 1},
  }, {
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
  }, {
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
  }, {
    {1, 1, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 1, 0},
    {0, 1, 0},
  }, {
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
  }, {
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
  }};
  void resetBallAndPaddle() {
    ball.setX(32);
    ball.setY(((rowStart + rowHeight * sizeof(rows) / sizeof(rows[0])) * 0.8 + 0.2)*64);
    ball.setVx(rand() % 2 == 0 ? slowSpeed / 2.0 : -slowSpeed / 2.0);
    ball.setVy(slowSpeed);
    volley = 0;
    score = 0;
    speed = slowSpeed;
  }
  void doVolley() {
    volley++;
    if (volley % 4 == 0 && speed < slowSpeed * 2.5) {
      speed += slowSpeed / 3.0;
    }
 
    int section = floor(ball.getX() - paddle.getX() - 1);
    
    if (section < 0)
      section = 0;
    if (section > 7)
      section = 7;
    
    ball.setVy(-speed);
    if (section == 0) {
      ball.setVx(-speed * 2.0);
    } else if (section == 1) {
      ball.setVx(-speed * 1.5);
    } else if (section == 2) {
      ball.setVx(-speed);
    } else if (section == 3) {
      ball.setVx(-speed*0.5);
    } else if (section == 4) {
      ball.setVx(speed*0.5);
    } else if (section == 5) {
      ball.setVx(speed);
    } else if (section == 6) {
      ball.setVx(speed*1.5);
    } else if (section == 7) {
      ball.setVx(speed*2);
    }

  }

  GameEntity paddle{32, 57.6, 10, 2, 255, true};
  GameEntity ball{32, 32, 2, 2, 255, true};

  bool rows[6][16] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  };
  float speed = 0.005;
  int volley = 0;
  int score = 0;

  void drawPattern(uint8_t * buffer, float x, float y, uint8_t v, const bool pattern[5][3]) {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 3; j++) {
        if(pattern[i][j]) {
            buffer[i * 64 + j] = v;
        }
      }
    }
  };

  void drawNumber(uint8_t * buffer, float x, float y, int number) {

    int reverse = 0;
    while (number != 0) {
      reverse = reverse * 10 + number % 10;
      number /= 10;
    }
    number = reverse;

    while (true) {
      int digit = number % 10;
      number /= 10;
      drawPattern(buffer, x, y, 255, numberPatterns[digit]);
      x += 4;
      if (number == 0) break;
    }

  };
};