#pragma once
#include "game.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm>
#include <iterator>

class Pacman : public Game {
  public:

    static constexpr uint8_t LEVEL_1[64][64] = {
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    class GhostHouse: public GameEntity {
      public:
        GhostHouse(float x, float y) : 
        GameEntity(x, y, 8, 4, boringColors[4], true) {}
    };

    class Player : public GameEntity {
      public:
        int rightBitmap[4][4] = {
          {1, 1, 1, 1},
          {1, 1, 0, 0},
          {1, 1, 0, 0},
          {1, 1, 1, 1}
        };
        int upBitmap[4][4] = {
          {1, 0, 0, 1},
          {1, 0, 0, 1},
          {1, 1, 1, 1},
          {1, 1, 1, 1}
        };
        int downBitmap[4][4] = {
          {1, 1, 1, 1},
          {1, 1, 1, 1},
          {1, 0, 0, 1},
          {1, 0, 0, 1}
        };
        int leftBitmap[4][4] = {
          {1, 1, 1, 1},
          {0, 0, 1, 1},
          {0, 0, 1, 1},
          {1, 1, 1, 1}
        };
        Action lastAction;
        Player(float x, float y) : 
        GameEntity(x, y, 4, 4, boringColors[2], true) {
          this->lastAction = UP;
          this->isSprite = true;
          sprite = (int*)upBitmap;
        }

        void move(Action action, float movementAmount, uint8_t levelLayout[64][64]) {
          bool valid = validMove(this, movementAmount, levelLayout, action);
          if(!valid) {
            if(validMove(this, movementAmount, levelLayout, this->lastAction)) {
              action = this->lastAction;
            } else {
              this->setVx(0);
              this->setVy(0);      
              this->lastAction = action;    
              return;
            }
          }

          if(action == UP) {
            if(this->getY()<=0) {
              this->setY(60);
            } else {
              this->setVy(-movementAmount);
            }
            this->setVx(0);
            sprite = (int*)upBitmap;
          } else if(action == LEFT) {
              if(this->getX()<=0) {
                this->setX(60);
              } else {
                this->setVx(-movementAmount);
              }
              this->setVy(0);
              sprite = (int*)leftBitmap;
          } else if(action == DOWN) {
              if(this->getY()>=64-this->getHeight()) {
                this->setY(0);
              } else {
                this->setVy(movementAmount);
              }
              this->setVx(0);
              sprite = (int*)downBitmap;
          } else if(action == RIGHT) {
              if(this->getX()>=64-this->getWidth()) {
                this->setX(0);
              } else {
                this->setVx(movementAmount);
              }
              this->setVy(0);
              sprite = (int*)rightBitmap;
          } else {
            this->setVx(0);
            this->setVy(0);
          }
          this->lastAction = action;      
        }

    };
    static bool validMove(GameEntity * gameEntity, float movementAmount, uint8_t level[64][64], Action action) {

      if(action == UP){

          int x1 = (int)std::min(std::max(gameEntity->getX(), 0.0f), 63.0f);
          int x2 = (int)std::min(std::max(gameEntity->getX()+gameEntity->getWidth(),0.0f), 63.0f);
          int y  = (int)std::min(std::max(gameEntity->getY()-std::ceil(movementAmount),0.0f), 63.0f);

          for (int x=x1; x<x2; x++) {
              if(level[y][x] > 0) {
                return false;
              }
          }
          return true;

      } else if(action == LEFT) {

        int y1 = (int)std::min(std::max(gameEntity->getY(), 0.0f), 63.0f);
        int y2 = (int)std::min(std::max(gameEntity->getY()+gameEntity->getHeight(),0.0f), 63.0f);
        int x  = (int)std::min(std::max(gameEntity->getX()-std::ceil(movementAmount),0.0f), 63.0f);

        for (int y=y1; y<y2; y++) {
            if(level[y][x] > 0) {
              return false;
            }
        }
        return true;

      } else if(action == DOWN) {

          int x1 = (int)std::min(std::max(gameEntity->getX(), 0.0f), 63.0f);
          int x2 = (int)std::min(std::max(gameEntity->getX()+gameEntity->getWidth(),0.0f), 63.0f);
          int y  = (int)std::min(std::max(gameEntity->getY()+gameEntity->getHeight(),0.0f), 63.0f);

          for (int x=x1; x<x2; x++) {
              if(level[y][x] > 0) {
                  return false;
              }
          }
          return true;

      } else if(action == RIGHT) {

          int y1 = (int)std::min(std::max(gameEntity->getY(), 0.0f), 63.0f);
          int y2 = (int)std::min(std::max(gameEntity->getY()+gameEntity->getHeight(),0.0f), 63.0f);
          int x  = (int)std::min(std::max(gameEntity->getX()+gameEntity->getWidth(),0.0f), 63.0f);

          for (int y=y1; y<y2; y++) {
              if(level[y][x] > 0) {
                  return false;
              }
          }
          return true;
      
      }

      return false;
          
    }

    static constexpr int boringColors[] = {75, 100, 150, 200, 225, 250};
    
    class Pellet : public GameEntity {
      public:
        bool isPower = false;
        int toggle = 0;
        Pellet(float x, float y, bool isPower) : 
        GameEntity(x, y, 2, 2,  boringColors[2], true) {
          this->isPower = isPower;
        }

        void update() {
          if(this->isPower) {
            this->toggle ^= 1;
            this->setColor(boringColors[2+this->toggle*2]);
          }
        }
    };

    class PelletManager {
      public:
        std::vector<Pellet*> powerPellets;
        std::vector<Pellet*> pellets;
        
        PelletManager(uint8_t levelLayout[64][64], Player * player, GhostHouse * ghostHouse, int powerPelletLocations[4][2]) {
          reset(levelLayout, player, ghostHouse, powerPelletLocations);
        }

        void spawnPellets(uint8_t levelLayout[64][64], Player * player, GhostHouse * ghostHouse, int powerPelletLocations[4][2]) {
          int noPelletLocations[5][2] = {
            {static_cast<int>(player->getX()), static_cast<int>(player->getY())}, 
            {static_cast<int>(ghostHouse->getX()), static_cast<int>(ghostHouse->getY())},
            {static_cast<int>(ghostHouse->getX()+4), static_cast<int>(ghostHouse->getY())},
            {static_cast<int>(ghostHouse->getX()), static_cast<int>(ghostHouse->getY()-4)},  
            {static_cast<int>(ghostHouse->getX()+4), static_cast<int>(ghostHouse->getY()-4)},  
          };


          for (int r=0; r<64; r++) {
            for (int c=0; c<64; c++) {

              bool noPellet = false;
              bool isPower = false;

              if(levelLayout[r][c] == 0 && r%4==0 && c%4==0) {

                for(int i=0; i<4; i++) {
                  if(c==powerPelletLocations[i][0] && r==powerPelletLocations[i][1]) {
                    isPower = true;
                    break;
                  }
                }
                for(int i=0; i<5; i++) {
                  if(c==noPelletLocations[i][0] && r==noPelletLocations[i][1]) {
                    noPellet = true;
                    break;
                  }
                }
                if(noPellet) {
                  continue;
                } else  {
                  Pellet * pellet = new Pellet(c+1, r+1, isPower);
                  if(isPower) {
                    powerPellets.push_back(pellet);
                  } else {
                    pellets.push_back(pellet);
                  }
                } 
              }
              
            }
          }
          //delete[] noPelletLocations;
          //delete[] powerPelletLocations;
        }

        void reset(uint8_t levelLayout[64][64], Player * player, GhostHouse * ghostHouse, int powerPelletLocations[4][2]) {
          if(pellets.size() > 0) {
            for(Pellet * pellet : pellets) {
              delete pellet;
            }
          }
          if(powerPellets.size() > 0) {
            for(Pellet * pellet : powerPellets) {
              delete pellet;
            }
          }
          pellets.clear();
          powerPellets.clear();
          spawnPellets(levelLayout, player, ghostHouse, powerPelletLocations);
        }
    };

    enum GhostState { SCATTER=0, CHASE=1, FRIGHTENED=2, NOT_ACTIVE=3, DEAD=4};
    
    static float euclidianDistance(float x1, float y1, float x2, float y2) {
      return std::sqrt(std::pow((x2-x1),2)+std::pow((y2-y1),2));
    }

    enum GameEvent { NOTHING=0, GHOST_KILLED=1, GAME_OVER=2};

    class Ghost : public GameEntity {
      public:
        int chaseBitmap[4][4] = {
          {1, 1, 1, 1},
          {0, 1, 1, 0},
          {1, 0, 0, 1},
          {1, 1, 1, 1}
        };
        int frightenedBitmap[4][4] = {
          {1, 1, 1, 1},
          {1, 0, 0, 1},
          {0, 1, 1, 0},
          {1, 1, 1, 1}
        };
        Action lastMove;
        int timer = 0;
        GhostState state = SCATTER;
        GhostState lastState;
        int scatterDuration = 210;
        int chaseDuration = 600;
        float normalSpeed = 0.25;
        float frightenedSpeed = 0.2;
        float initialX;
        float initialY;
        int reviveTimer = 0;
        int reviveDuration = 150;
        int limit;
        Ghost(float x, float y, GhostState state, int limit) : 
        GameEntity(x, y, 4, 4, boringColors[4], true) {
          this->isSprite = true;
          sprite = (int*)chaseBitmap;
          this->state = state;
          this->limit = limit;
          this->initialX = x;
          this->initialY = y;
        }

        GameEvent update(Player * player, uint8_t levelLayout[64][64], bool isFrightened, int pelltsConsumed, Ghost * ghost) {

          if(state == NOT_ACTIVE) {
            if(pelltsConsumed > this->limit) {
              state = SCATTER;
            } else {
              return NOTHING;
            }
          }

          if(state == DEAD) {
            reviveTimer += 1;
            if(reviveTimer < reviveDuration) {
              return NOTHING;
            } else {
              state = SCATTER;
            }
          }

          if(collide(player)) {
           
            if(isFrightened || state==FRIGHTENED) {
              reviveTimer = 0;
              state = DEAD;
              setX(initialX);
              setY(initialY);
              sprite = (int*)chaseBitmap;
              return GHOST_KILLED;
            } else {
              return GAME_OVER;
            }
          }   
        
          if(state == FRIGHTENED && isFrightened) {
            
            randomMove(levelLayout);
            return NOTHING;
          }

          if(state == FRIGHTENED && !isFrightened) {
            state = lastState;
            sprite = (int*)chaseBitmap;
          }

          if(isFrightened) {
            lastState = state;
            state = FRIGHTENED;
            sprite = (int*)frightenedBitmap;
            if (lastMove == UP) {
              setY(getY()+frightenedSpeed);
              lastMove = DOWN;
            } else if (lastMove == LEFT) {
              setX(getX()+frightenedSpeed);
              lastMove = RIGHT;              
            } else if (lastMove == DOWN) {
              setY(getY()-frightenedSpeed);
              lastMove = UP;             
            } else if(lastMove == RIGHT) {
              setX(getX()-frightenedSpeed);
              lastMove = LEFT;               
            }
          } else if(state == SCATTER) {
            timer += 1;
            scatter(levelLayout);

            if(timer > scatterDuration) {
              timer = 0;
              state = CHASE;
            }
          } else if(state == CHASE) {
            timer += 1;
            chase(player, ghost, levelLayout);
            if(timer > chaseDuration) {
              timer = 0;
              state = SCATTER;
            }
          }

          return NOTHING;
        }

        int argmin(float distance[4]) {
          float minValue = 999.0;
          int minIndex = 0;
          for(int i=0; i<4; i++) {
            if(distance[i] < minValue) {
              minValue = distance[i];
              minIndex = i;
            }
          }
          return minIndex;
        }
      
        void followTarget(float x, float y, uint8_t levelLayout[64][64]) {
          float distance[4] = {999.0,999.0,999.0,999.0};

          float x1 = std::min(std::max(this->getX(), 0.0f), 63.0f);
          float y1 = std::min(std::max(this->getY(), 0.0f), 63.0f);
          float x2 = std::min(std::max(this->getWidth()+x1, 0.0f), 63.0f);
          float y2 = std::min(std::max(this->getHeight()+y1, 0.0f), 63.0f);

          float nextStep = std::ceil(normalSpeed);

          if(lastMove != DOWN && validMove(this, nextStep, levelLayout, UP)) {
            distance[0] = euclidianDistance(this->getX(), this->getY()-nextStep, x, y);
          }

          if(lastMove != RIGHT && validMove(this, nextStep, levelLayout, LEFT)) {
            distance[1] = euclidianDistance(this->getX()-nextStep, this->getY(), x, y);
          }

          if(lastMove != UP && validMove(this, nextStep, levelLayout, DOWN)) {
            distance[2] = euclidianDistance(this->getX(), this->getY()+nextStep, x, y);
          }

          if(lastMove != LEFT && validMove(this, nextStep, levelLayout, RIGHT)) {
            distance[3] = euclidianDistance(this->getX()+nextStep, this->getY(), x, y);
          }

          int minimum = argmin(distance);
        
          if(minimum==0) {
            this->setY(this->getY()-normalSpeed);
            this->lastMove = UP;
          } else if(minimum==1) {
            this->setX(this->getX()-normalSpeed);
            this->lastMove = LEFT;  
          } else if(minimum==2) {
            this->setY(this->getY()+normalSpeed);
            this->lastMove = DOWN;
          } else if(minimum==3) {
            this->setX(this->getX()+normalSpeed);
            this->lastMove = RIGHT;      
          }

        }

        virtual void scatterTarget(float &x, float &y) = 0;
        virtual void chase(Player * player, Ghost * blinky, uint8_t levelLayout[64][64]) = 0;

        void scatter(uint8_t levelLayout[64][64]) {
          float x, y;
          scatterTarget(x, y);
          followTarget(x, y, levelLayout);
        }

        void randomMove(uint8_t levelLayout[64][64]) {
          std::vector<int> moves{0, 0, 0, 0};

          if(validMove(this, frightenedSpeed, levelLayout, UP)) {
            moves[0] += 1;
            if(this->lastMove == UP) {
              moves[0] += 20;
            } 
          }

          if(validMove(this, frightenedSpeed, levelLayout, LEFT)) {
            moves[1] += 1;
            if(this->lastMove == LEFT) {
              moves[1] +=20;
            } 
          }

          if(validMove(this, frightenedSpeed, levelLayout, DOWN)) {
            moves[2] += 1;
            if(this->lastMove == DOWN) {
              moves[2] +=20;
            } 
          }
  
          if(validMove(this, frightenedSpeed, levelLayout, RIGHT)) {
            moves[3] += 1;
            if(this->lastMove == RIGHT) {
              moves[3] +=20;
            } 
          }
          
          int index = pmf(moves);

          if(index==0) {
            setY(getY()-frightenedSpeed);
            this->lastMove = UP;
          } else if(index==1) {
            setX(getX()-frightenedSpeed);
            this->lastMove = LEFT;              
          } else if(index==2) {
            setY(getY()+frightenedSpeed);
            this->lastMove = DOWN;
          } else if(index==3) {
            setX(getX()+frightenedSpeed);
            this->lastMove = RIGHT;
          }
        }
    };

    class Blinky : public Ghost {
      public:
        Blinky(float x, float y, GhostState state) : 
        Ghost(x, y, state, 0) {}

        void scatterTarget(float &x, float &y) {
          x = 64;
          y = 0;
        };

        void chase(Player * player, Ghost * ghost, uint8_t levelLayout[64][64]) {
          followTarget(player->getX(), player->getY(), levelLayout);
        }
    };

    class Inky : public Ghost {
      public:
        Inky(float x, float y, GhostState state, int limit) : 
        Ghost(x, y, state, limit) {}

        void scatterTarget(float &x, float &y) {
          x = 8;
          y = -4;
        };

        void chase(Player * player, Ghost * blinky, uint8_t levelLayout[64][64]) {
          float targetX = player->getX();
          float targetY = player->getY();
          if(lastMove == UP) {
            targetY -= 2;
          } else if(lastMove == LEFT) {
            targetX -= 2;
          } else if(lastMove == DOWN) {
            targetY += 2;
          } else if(lastMove == RIGHT) {
            targetX += 2;     
          }

          targetX = targetX -1*(targetX-blinky->getX());
          targetY = targetY -1*(targetY-blinky->getY());

          followTarget(targetX, targetY, levelLayout);
        }
    };

    class Pinky : public Ghost {
      public:
        Pinky(float x, float y, GhostState state) : 
        Ghost(x, y, state, 0) {}

        void scatterTarget(float &x, float &y) {
          x = 0;
          y = 0;
        };

        void chase(Player * player, Ghost * ghost, uint8_t levelLayout[64][64]) {
          
          float targetX;
          float targetY;
          if(lastMove == UP) {
            targetX = player->getX();
            targetY = player->getY()-4;
          } else if(lastMove == LEFT) {
            targetX = player->getX()-4;
            targetY = player->getY();
          } else if(lastMove == DOWN) {
            targetX = player->getX();
            targetY = player->getY()+4;
          } else if(lastMove == RIGHT) {
            targetX = player->getX()+4;
            targetY = player->getY();            
          }

          followTarget(targetX, targetY, levelLayout);
        }
    };

    class Clyde : public Ghost {
      public:
        Clyde(float x, float y, GhostState state, int limit) : 
        Ghost(x, y, state, limit) {}

        void scatterTarget(float &x, float &y) {
          x = 0;
          y = 64;
        };

        void chase(Player * player, Ghost * ghost, uint8_t levelLayout[64][64]) {
          
          float distance = euclidianDistance(getX(), getY(), player->getX(), player->getY());

          if(distance > 8) {
            followTarget(player->getX(), player->getY(), levelLayout);
          } else {
            float x, y;
            scatterTarget(x, y);
            followTarget(x, y, levelLayout);
          }

        }
    };

    class GhostManager {
      public:
        Blinky * blinky = nullptr;
        Pinky * pinky = nullptr;
        Inky * inky = nullptr;
        Clyde * clyde = nullptr;
        std::vector<Ghost*> ghosts;
        GhostManager(GhostHouse * ghostHouse) {
          reset(ghostHouse);
        }

        void reset(GhostHouse * ghostHouse) {

          if(blinky || pinky || inky || clyde) {
            ghosts.clear();
            delete blinky;
            delete pinky; 
            delete inky; 
            delete clyde;
          }

          float x = ghostHouse->getX();
          float y = ghostHouse->getY();
          blinky = new Blinky(x, y-4, SCATTER);
          pinky = new Pinky(x+4, y-4, SCATTER);
          inky = new Inky(x, y, NOT_ACTIVE, 30);
          clyde = new Clyde(x, y, NOT_ACTIVE, 60);
          ghosts = {blinky, pinky, inky, clyde};
        }

        void nextLevel(int level, GhostHouse * ghostHouse) {
          for(Ghost * ghost : ghosts) {
            if(level > 3) {
              ghost->scatterDuration -= 10;
              ghost->chaseDuration += 10;
            }

            if(level <  5) {
              ghost->normalSpeed += 0.05;
              ghost->frightenedSpeed += 0.05;
            }
          }

          reset(ghostHouse);
        }

        GameEvent update(Player * player, uint8_t levelLayout[64][64], bool frightened, int pelletsConsumed) {
          GameEvent event = NOTHING;
          for (Ghost * ghost : ghosts) {
            GameEvent nextGameEvent = ghost->update(player, levelLayout, frightened, pelletsConsumed, blinky);
            if(nextGameEvent == GAME_OVER) {
              return nextGameEvent;
            } else if(nextGameEvent == NOTHING) {
              event = nextGameEvent;
            }
          }
          return event;
        }
    };
    private:
      uint8_t  levelLayout[64][64];

      Player * player;
      GhostHouse * ghostHouse;
      PelletManager * pelletManager;
      GhostManager * ghostManager;
      bool isEnergized = false;
      int energizedTimer = 0;
      int energizedDuration = 300;
      int pelletsConsumed = 0;
      int pelletAmount;
      int ghostKillStreak = 0;
      int level = 1;
      Action lastAction = NO_ACTION;
    public:
      Pacman() : Game() {
        initLevel(LEVEL_1);
        player = new Player(28, 56);
        ghostHouse = new GhostHouse(28, 28);
        int powerPelletLocations[4][2] = {{4, 4},{56,4},{4, 44},{56, 44}};
        pelletManager = new PelletManager(levelLayout, player, ghostHouse, powerPelletLocations);
        ghostManager = new GhostManager(ghostHouse);
        pelletAmount = pelletManager->pellets.size();
      }

      void initLevel(uint8_t const LEVEL[64][64]) {
        std::memcpy(&levelLayout[0][0], LEVEL, sizeof(uint8_t) * 64 * 64); 
    
        for(int r=0; r<64; r++) {
          for(int c=0; c<64; c++) {
            levelLayout[r][c] *= 255;
          }
        }
      }

      void nextLevel() {
        level++;
        energizedTimer = 0;
        isEnergized = false;
        ghostKillStreak = 0;
        pelletsConsumed = 0;
        player->setX(28);
        player->setY(56);

        ghostManager->nextLevel(level, ghostHouse); 
        int powerPelletLocations[4][2] = {{4, 4},{56,4},{4, 44},{56, 44}};
        pelletManager->reset(levelLayout, player, ghostHouse, powerPelletLocations);
        pelletAmount = pelletManager->pellets.size();
      }

      std::vector<bool> legalActions() override {
        //      NOOP, FIRE, UP,   DOWN,   LEFT, RIGHT
        return {true, false, true, true,  true, true};
      }

      void renderPixels(uint8_t * buffer) override {
        std::memcpy(buffer, &levelLayout, sizeof(uint8_t) * 64 * 64); 
        for(Pellet * pellet: pelletManager->pellets){
          pellet->render(buffer);
        }
        for(Pellet * pellet: pelletManager->powerPellets){
          pellet->render(buffer);
        }
        player->render(buffer);
        ghostHouse->render(buffer);
        for(Ghost * ghost : ghostManager->ghosts) {
          ghost->render(buffer);
        }
      }

      std::vector<int> step(uint8_t action) override {
        int score = 0;
        bool Up = (action == 2);
        bool Down = (action == 3);
        bool Left = (action == 4);
        bool Right = (action == 5); 

        if(action >=2 && action <= 5) {
          player->move((Action)action, 0.5, levelLayout);
          lastAction = (Action)action;
          player->update();
        } else if(lastAction == NO_ACTION) {
          
        } else {
          player->move(lastAction, 0.4, levelLayout);
          player->update();
        }

        for(int i=0; i<pelletManager->pellets.size(); i++) {
          Pellet * pellet = pelletManager->pellets[i];
          if(pellet->getIsAlive() && pellet->collide(*player)) {
            pelletManager->pellets.erase(pelletManager->pellets.begin()+i);
            pelletsConsumed += 1;
            score += 10;

            if(pelletsConsumed == pelletAmount) {
              nextLevel();
              return {score, false};
            }
          }  
        }

        if(isEnergized) {
          if(energizedTimer < energizedDuration) {
            energizedTimer += 1;
          } else {
            energizedTimer = 0;
            isEnergized = false;
            ghostKillStreak = 0;
          }
        }

        for(int i=0; i<pelletManager->powerPellets.size(); i++) {
          Pellet * powerPellet = pelletManager->powerPellets[i];
          if(powerPellet->getIsAlive()) {
            if(powerPellet->collide(player)) {
              powerPellet->setIsAlive(false);

              if(isEnergized) {
                energizedTimer -= energizedDuration;
              } else {
                isEnergized = true;
              }
              
              score += 50;
            } else {
              powerPellet->update();
            }

          }  
        }

        GameEvent event = ghostManager->update(player, levelLayout, isEnergized, pelletsConsumed);

        if (event == GHOST_KILLED) {
            score += 200*std::pow(2, ghostKillStreak);
            ghostKillStreak += 1;
        } else if(event == GAME_OVER) {
          return {score, true};
        }


        return {score, false};
      }
};