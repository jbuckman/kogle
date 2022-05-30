#pragma once
#include "game.h"
#include <math.h>
#include <iostream>
#include <vector>

class Pacman : public Game {

  Pacman() : Game() {}
  std::vector<bool> legalActions() override {
    //      NOOP, FIRE, UP,   DOWN,   LEFT, RIGHT
    return {true, true, false, false,  true, true};
  }

  void renderPixels(uint8_t * buffer) override {
    memset(buffer, 0, sizeof(uint8_t) * 64 * 64);
  }

  std::vector<int> step(uint8_t action) override {
    return {0, false};
  }
};