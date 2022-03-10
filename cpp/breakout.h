#pragma once
#include "game.h"
#include <math.h>
constexpr float slowSpeed = 0.35 / 64.0;
constexpr float paddleSpeed = 0.5 / 64.0;
constexpr float paddleSize = 10.0 / 64.0;
constexpr float ballSize = 2.0 / 64.0;

constexpr float rowHeight = 2.0 / 64.0;
constexpr float rowStart = 20.0 / 64.0;

constexpr int scores[] = {7, 7, 4, 4, 1, 1};
constexpr int boringColors[] = {75, 100, 150, 200, 225, 250};
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
struct Rect {
  float x;
  float y;
  float width;
  float height;
};
inline bool collide(const Rect& a, const Rect& b) {
  return a.x + a.width / 2.0 > b.x - b.width / 2.0 &&
         a.x - a.width / 2.0 < b.x + b.width / 2.0 &&
         a.y + a.height / 2.0 > b.y - b.height / 2.0 &&
         a.y - a.height / 2.0 < b.y + b.height / 2.0;
}
class Breakout : public Game {
public:
  Breakout() {
    reset();
  }
  std::vector<bool> legalActions() override {
    //      Up     Down   Left  Right Fire
    return {false, false, true, true, false};
  }
  void renderPixels(uint8_t * buffer) override {
    memset(buffer, 0, sizeof(uint8_t) * 64 * 64);
    auto drawPixel = [&](float x, float y, uint8_t v) {
      if (x < 0 || x >= 64 || y < 0 || y >= 64) return;
      int i = floor(y) * 64 + floor(x);
      buffer[i] = v;
    };
    auto drawRect = [&](float x, float y, float w, float h, uint8_t v) {
      for (float i = x; i < x + w; i++) {
        for (float j = y; j < y + h; j++)
          drawPixel(i, j, v);
      }
    };
    auto drawRectDangerously = [&](float x, float y, float w, float h, uint8_t v) {
      for (float j = y; j < y + h; j++) {
        for (float i = x; i < x + w; i++) {
          int index = floor(j) * 64 + floor(i);
          buffer[index] = v;
        }
      }
    };
    auto drawPattern = [&](float x, float y, uint8_t v, const bool pattern[5][3]) {
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
          if (pattern[i][j]) {
            buffer[i * 64 + j] = v;
          }
        }
      }
    };
    auto drawNumber = [&](float x, float y, int number) {
      {
        int reverse = 0;
        while (number != 0) {
          reverse = reverse * 10 + number % 10;
          number /= 10;
        }
        number = reverse;
      }
      while (true) {
        int digit = number % 10;
        number /= 10;
        drawPattern(x, y, 255, numberPatterns[digit]);
        x += 4;
        if (number == 0) break;
      }
    };
    drawRect(64.0 * paddle.x - paddleSize * 64.0 / 2.0, 64.0 * paddle.y - ballSize * 64.0 / 2.0, paddleSize * 64.0, ballSize * 64.0, 255);
    drawRect(64.0 * ball.x - ballSize * 64.0 / 2.0, 64.0 * ball.y - ballSize * 64.0 / 2.0, ballSize * 64.0, ballSize * 64.0, 255);
    drawPixel(ball.x * 64.0, ball.y * 64.0, 255);
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
          drawRectDangerously(64.0 * x / rowSize, 64.0 * y, 64.0 / rowSize, 64.0 * rowHeight, 0);
      }
      y += rowHeight;
    }
    drawNumber(1, 1, score);
  }
  std::vector<int> step(uint8_t action) override {
    bool Up = (action == 2) || (action == 2 + 4);
    bool Down = (action == 3) || (action == 3 + 4);
    bool Left = (action == 4) || (action == 4 + 4);
    bool Right = (action == 5) || (action == 5 + 4);
    bool Fire = (action == 0) || (action > 5);
    float lastScore = this->score;
    ball.x += ball.vx;
    ball.y += ball.vy;

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
      humanSpeed += paddleSpeed;
    if (Left)
      humanSpeed -= paddleSpeed;
    paddle.x += humanSpeed;
    if (collide({paddle.x, paddle.y, paddleSize, ballSize}, {ball.x, ball.y, ballSize, ballSize})) {
      doVolley();
    }
    float y = rowStart;
    for (int i = 0; i < sizeof(rows) / sizeof(rows[0]); i++) {
       auto& row = rows[i];
       for (int x = 0; x < sizeof(row) / sizeof(row[0]); x++) {
        if (!row[x])
          continue;
        if (collide({(float) x / 16.0f + 0.5f / 16.0f, y + rowHeight / 2.0f, 1.0 / 16.0f, rowHeight}, {ball.x, ball.y, ballSize, ballSize})) {
          row[x] = 0;
          ball.vy = -ball.vy;
          score += scores[i];
          goto outer;
        }
      }
      y += rowHeight;
    }
    outer:;
    if (ball.y < 0) {
      ball.y = 0;
      ball.vy = -ball.vy;
    }
    if (ball.x > 1) {
      ball.x = 1;
      ball.vx = -ball.vx;
    }
    if (ball.x < 0) {
      ball.x = 0;
      ball.vx = -ball.vx;
    }
    paddle.x = std::max(0.0f, std::min(1.0f, paddle.x));
    const bool isDone = ball.y > 1;
    const int score = this->score - lastScore;
    if (isDone)
      reset();
    return {score, isDone};
  }
  void reset() override {
    ball.x = 0.5;
    ball.y = (rowStart + rowHeight * sizeof(rows) / sizeof(rows[0])) * 0.8 + 0.2;
    ball.vx = rand() % 2 == 0 ? slowSpeed / 2.0 : -slowSpeed / 2.0;
    ball.vy = slowSpeed;
    volley = 0;
    score = 0;
    speed = slowSpeed;
  }
private:
  void doVolley() {
    volley++;
    if (volley % 4 == 0 && speed < slowSpeed * 2.5) {
      speed += slowSpeed / 3.0;
    }
    const float left = paddle.x - paddleSize / 2.0;
    int section = floor(8.0 * (ball.x - left) / paddleSize);
    if (section < 0)
      section = 0;
    if (section > 7)
      section = 7;
    ball.vy = -speed;
    if (section == 0) {
      ball.vx = -speed * 2.0;
    } else if (section == 1) {
      ball.vx = -speed * 1.5;
    } else if (section == 2) {
      ball.vx = -speed;
    } else if (section == 3) {
      ball.vx = -speed * 0.5;
    } else if (section == 4) {
      ball.vx = speed * 0.5;
    } else if (section == 5) {
      ball.vx = speed;
    } else if (section == 6) {
      ball.vx = speed * 1.5;
    } else if (section == 7) {
      ball.vx = speed * 2.0;
    }
  }
  struct Paddle {
    float x;
    float y;
  };
  Paddle paddle = {0.5, 0.9};
  struct Ball {
    float x;
    float y;
    float vx;
    float vy;
  };
  Ball ball = {0.5, rowHeight, 0.005, 0.005};
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
};
