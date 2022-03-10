#pragma once
#include <vector>
class Game {
public:
  virtual std::vector<bool> legalActions() = 0;
  virtual void renderPixels(uint8_t * buffer) = 0;
  virtual std::vector<int> step(uint8_t action) = 0;
};
