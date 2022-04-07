#pragma once
#include <vector>
#include <time.h>
#include <random>

class Game {

public:

  virtual std::vector<bool> legalActions() = 0;
  virtual void renderPixels(uint8_t * buffer) = 0;
  virtual std::vector<int> step(uint8_t action) = 0;
  
  Game() {
    srand(time(NULL));
  }

protected:

  struct Rect {
    float x;
    float y;
    float width;
    float height;
  };

  struct Point {
    float x;
    float y;
  };

  enum Direction {
    LEFT,
    RIGHT,
    NA
  };

  struct GameEntity {
    
    Rect rect;
    int width, height;
    std::vector<std::vector<bool>>  sprite;
    Direction direction;
    bool isAlive;
    
    GameEntity(float x, float y, std::vector<std::vector<bool>> bitmap, bool isAlive, Direction direction=NA):
    width(bitmap[0].size()), 
    height(bitmap.size()), 
    sprite(bitmap), 
    direction(direction), 
    isAlive(isAlive) {

      rect = {x, y, width/64.0f, height/64.0f};
    }
  };
  
  void drawPixel(uint8_t * buffer, float x, float y, uint8_t v) {
    if (x < 0 || x >= 64 || y < 0 || y >= 64) return;
    int i = floor(y) * 64 + floor(x);
    buffer[i] = v;
  };

  void drawSprite(uint8_t * buffer, const GameEntity& entity, uint8_t v) {
    
    for (int r=0; r<entity.height; r++) { 
      for (int c = 0; c <entity.width; c++) { 
        if(entity.direction == NA || entity.direction == LEFT) {
          if(entity.sprite[r][c]) {
            drawPixel(buffer, entity.rect.x*64.0 + c-entity.width/2.0, entity.rect.y*64.0+r, v);
          }
        } else {
          if(entity.sprite[r][entity.width-c-1]) {
            drawPixel(buffer, entity.rect.x*64.0 + c-entity.width/2.0, entity.rect.y*64.0+r, v);
          }    
        }
      }
    }
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

  void drawVector(uint8_t * buffer,  std::vector<std::vector<bool>> bitmap, float x, float y, uint8_t v) {
    for (int r=0; r<bitmap.size(); r++) { 
      for (int c = 0; c <bitmap[r].size(); c++) { 
        if(bitmap[r][c]) {
          drawPixel(buffer, x*64.0 + c-bitmap.size()/2.0, y*64.0+r, v);
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

  bool collide(const Rect& a, const Rect& b) {
    return 
      a.x + a.width / 2.0 > b.x - b.width / 2.0 &&
      a.x - a.width / 2.0 < b.x + b.width / 2.0 &&
      a.y + a.height / 2.0 > b.y - b.height / 2.0 &&
      a.y - a.height / 2.0 < b.y + b.height / 2.0;
  };

  bool bernoulli(double probability) {
    std::random_device randomDevice{}; 
    std::mt19937 generator{randomDevice()}; 
    std::bernoulli_distribution distribution(probability);
    return distribution(generator);
  }

  double uniform(double min, double max) {
    std::random_device randomDevice{}; 
    std::mt19937 generator{randomDevice()}; 
    std::uniform_real_distribution distribution(min, max);
    return distribution(generator);
  }

  int randomBetween(int min, int max) {
    return (rand() % max + min);
  }

  int diceRoll() {
    return randomBetween(1,6);
  }
  
};
