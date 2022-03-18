#pragma once
#include <vector>
#include "constants.h"


class Game {
public:

  virtual std::vector<bool> legalActions() = 0;
  virtual void renderPixels(uint8_t * buffer) = 0;
  virtual std::vector<int> step(uint8_t action) = 0;

protected:

  void drawPixel(uint8_t * buffer, float x, float y, uint8_t v) {
    if (x < 0 || x >= 64 || y < 0 || y >= 64) return;
    int i = floor(y) * 64 + floor(x);
    buffer[i] = v;
  };
  
  template <size_t size_x, size_t size_y>
  void drawBitmap(uint8_t * buffer,  bool (&bitmap)[size_x][size_y], float x, float y, uint8_t v) {
    for (int r=0; r<size_x; r++) { 
      for (int c = 0; c <size_y; c++) { 
        if(bitmap[r][c]) {
          drawPixel(buffer, x*64.0 + c-size_x/2.0, y*64.0+r, v);
        }
      }
    }
  };

  void drawRect(uint8_t * buffer, float x, float y, float w, float h, uint8_t v) {
    for (float i = x; i < x + w; i++) {
      for (float j = y; j < y + h; j++) {
          drawPixel(buffer, i, j, v);
      }
    }
  };

  void drawRectDangerously(uint8_t * buffer, float x, float y, float w, float h, uint8_t v) {
    for (float j = y; j < y + h; j++) {
      for (float i = x; i < x + w; i++) {
        int index = floor(j) * 64 + floor(i);
        buffer[index] = v;
      }
    }   
  };

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

  bool collide(const Rect& a, const Rect& b) {
    return 
      a.x + a.width / 2.0 > b.x - b.width / 2.0 &&
      a.x - a.width / 2.0 < b.x + b.width / 2.0 &&
      a.y + a.height / 2.0 > b.y - b.height / 2.0 &&
      a.y - a.height / 2.0 < b.y + b.height / 2.0;
  };
  
};
