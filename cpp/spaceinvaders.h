#pragma once
#include "game.h"
#include <math.h>
#include <iostream>


class Spaceinvaders : public Game {
public:
  Spaceinvaders() : Game() {
    reset();
  }
  std::vector<bool> legalActions() override {
    //      NOOP, FIRE,   UP,   DOWN,   LEFT, RIGHT, UPFIRE, DOWNFIRE, LEFTFIRE, RIGHTFIRE
    return {true, true, false, false,  true, true,   false,   false,   false,    false};
  }
  void renderPixels(uint8_t * buffer) override {
    memset(buffer, 0, sizeof(uint8_t) * 64 * 64);

    drawBitmap(buffer, shipPatternRows, ship.x, ship.y, 255);

    for(ShieldBlock &block: shieldBlocks) {
      if(block.health > 0) {
        drawBitmap(buffer, SHIELD_BLOCK_STATES[block.health-1], block.point.x, block.point.y, 255);
      }  
    }

    for(int r=0; r < ENEMY_ROWS; r++) {
      for(int c=0; c < ENEMIES_PER_ROW; c++) {
        if(enemyGrid[r][c]) {
          drawBitmap(buffer, ENEMYSPRITE, enemies[r][c].x, enemies[r][c].y, 255);
        }
      }
    }

    for(int i=0; i < maxEnemyBullets; i++) {
      if(enemyBullets[i].active) {
        drawRect(buffer, enemyBullets[i].point.x*64.0, enemyBullets[i].point.y*64.0, 64.0*bulletWidth, 64.0*bulletHeight, 255);
      }
    }

    if(bulletFired) {
      drawRect(buffer, bullet.x*64.0, bullet.y*64.0, 64.0*bulletWidth, 64.0*bulletHeight, 255);
    }
  }

  std::vector<int> step(uint8_t action) override {

    int score = 0;
    bool gameOver = false;

    bool Left = (action == 4) | (action == 4 + 4);
    bool Right = (action == 5) | (action == 5 + 4);
    bool Fire = (action == 1);

    if (Right) {
      ship.x += shipSpeed;
    } else if (Left) {
      ship.x -= shipSpeed;
    }
    
    ship.x = std::max(0.0f, std::min(1.0f, ship.x));

    for(int c=ENEMIES_PER_ROW-1; c >= 0; c--) {
      for(int r=ENEMY_ROWS-1; r >= 0; r--) {
        if(enemyGrid[r][c]) {
          if(enemies[r][c].x + enemySpeedX >= 1.0  && enemyMoveRight || enemies[r][c].x - enemySpeedX <= 0.0  && !enemyMoveRight) {
            
            if(enemies[r][c].y + enemySpeedY >= 0.7) {
              return {score, true};
            }

            enemyMoveRight = !enemyMoveRight;
            for(auto & row : enemies) {
              for(auto & enemy : row) {
                enemy.y += enemySpeedY;
              }
            } 
          }
     
          if(enemyMoveRight)
            enemies[r][c].x += enemySpeedX;
          else 
            enemies[r][c].x -= enemySpeedX; 
        }
      }
    }

    if(enemyBulletsActive < maxEnemyBullets && bernoulli(enemyFireProbability)) {
      
      for(int r=0; r <ENEMY_ROWS; r++) {
        std::vector<int> aliveInRow;
      
        for(int c=0; c < ENEMIES_PER_ROW; c++) {
    
          if(enemyGrid[r][c]) {
            aliveInRow.push_back(c);
          }
        }
        int aliveCount =  aliveInRow.size();
        if(aliveCount>0) {
          int randomIndex = rand() % aliveCount;
          int c = aliveInRow[randomIndex];

          for(int i=0; i < maxEnemyBullets; i++) {
            if(!enemyBullets[i].active) {
              enemyBullets[i] = {{enemies[r][c].x+(ENEMY_WIDTH/4.0f)/64.0f, enemies[r][c].y}, true};
              enemyBulletsActive++;
              break;
            }
          }
          break;
        }
      }
    }

    for(int i=0; i < maxEnemyBullets; i++) {
      if(enemyBullets[i].active) {
        
        bool hit = false;
        
        for(ShieldBlock & block: shieldBlocks) {
          if (block.health > 0 && collide({block.point.x, block.point.y, SHIELD_BLOCK_DIM, SHIELD_BLOCK_DIM}, {enemyBullets[i].point.x, enemyBullets[i].point.y, bulletWidth, bulletHeight})) {
            block.health =  block.health - 1;
            hit = true;
            break;
          }
        }

        if(!hit && collide({ship.x, ship.y, SHIP_WIDTH/64.0f, SHIP_HEIGHT/64.0f}, {enemyBullets[i].point.x, enemyBullets[i].point.y, bulletWidth, bulletHeight})) {
          return {score, true};
        }
        
        enemyBullets[i].point.y += enemyBulletSpeed;
        
        if(hit || enemyBullets[i].point.y >= 1.0) {
          enemyBulletsActive--;
          enemyBullets[i].active = false;
        }
      }
    }

    if (Fire && !bulletFired) {
      
      bullet = {ship.x+(SHIP_WIDTH/4.0f)/64.0f, ship.y};
      bulletFired = true;
    } 

    if (bulletFired) {
      bool hit = false;
      bullet.y -= playerBulletSpeed;

      for(int r=0; r < ENEMY_ROWS; r++) {
        for(int c=0; c < ENEMIES_PER_ROW; c++) {
          if(enemyGrid[r][c] && collide({enemies[r][c].x, enemies[r][c].y, 5.0f/64.0f, 5.0f/64.0f}, {bullet.x, bullet.y, bulletWidth, bulletHeight})) {
            hit = true;
            score++;
            enemiesKilled++;
            enemyGrid[r][c] = false;  
            break;    
          }
        }
      }

      if(enemiesKilled == TOTAL_ENEMIES) {
        level++;
        reset();
        return {++score, false};
      }
      if(!hit) {
        for(ShieldBlock & block: shieldBlocks) {
          if (block.health > 0 && collide({block.point.x, block.point.y, SHIELD_BLOCK_DIM, SHIELD_BLOCK_DIM}, {bullet.x, bullet.y, bulletWidth, bulletHeight})) {
            block.health =  block.health - 1;
            hit = true;
            break;
          }
        }
      }

      if(bullet.y < 0.0 || hit) {
 
        bulletFired = false;
      }
    }    
    return {score, gameOver};
  }

private:

  static constexpr float slowSpeed = 0.35 / 64.0;
  static constexpr float shipSpeed = 0.5 / 64.0;

  static constexpr float rowHeight = 2.0 / 64.0;
  static constexpr float rowStart = 20.0 / 64.0;

  static constexpr float bulletHeight = 2.0 / 64.0;
  static constexpr float bulletWidth = 1.0 / 64.0;

  static constexpr float SHIELD_BLOCK_DIM = 3.0 / 64.0;

  static constexpr int ENEMY_ROWS = 4;
  static constexpr int ENEMIES_PER_ROW = 6;
  
  static constexpr int TOTAL_ENEMIES = ENEMY_ROWS * ENEMIES_PER_ROW;

  bool shipPatternRows[3][7] = {
    {0, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},  
  };
  
  bool ENEMYSPRITE[4][5] = {
    {1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1}
  };

  int SHIP_HEIGHT =  sizeof(shipPatternRows)/sizeof(shipPatternRows[0]); 
  int SHIP_WIDTH =  sizeof(shipPatternRows[0])/sizeof(bool); 

  int ENEMY_HEIGHT =  sizeof(ENEMYSPRITE)/sizeof(ENEMYSPRITE[0]); 
  int ENEMY_WIDTH =  sizeof(ENEMYSPRITE[0])/sizeof(bool); 

  Point ship = {0.5, 0.9};

  bool bulletFired = false;

  float playerBulletSpeed = 0.0025f;
  float enemyBulletSpeed = 0.0025f;

  Point bullet = {0.5, 1.0};
  
  struct EnemyBullet {
    Point point;
    bool active;
  };

  std::vector<EnemyBullet> enemyBullets;

  int maxEnemyBullets = 1;
  int enemyBulletsActive = 0;
  
  float enemySpeedX = 0.001;
  float enemySpeedY = 0.01;
  bool  enemyMoveRight = true;

  struct ShieldBlock {
    Point point;
    int health;
  };

  bool SHIELD_BLOCK_STATES[3][3][3] = {
    {
      {0, 0, 0},
      {1, 0, 1},
      {1, 1, 0}
    },
    {
      {0, 1, 1},
      {1, 0, 1},
      {1, 1, 0}
    },
    {
      {1, 1, 1},
      {1, 1, 1},
      {1, 1, 1}
    },
  };

  ShieldBlock shieldBlocks[8] = {
    {{0.1f,0.8f},3},
    {{0.1f+SHIELD_BLOCK_DIM,0.8f},3},

    {{0.35f,0.8f},3},
    {{0.35f+SHIELD_BLOCK_DIM,0.8f},3},

    {{0.6f,0.8f},3},
    {{0.6f+SHIELD_BLOCK_DIM,0.8f},3},

    {{0.85f,0.8f},3},
    {{0.85f+SHIELD_BLOCK_DIM,0.8f},3},
  };

  bool enemyGrid[ENEMY_ROWS][ENEMIES_PER_ROW];

  int enemiesKilled = 0;
  int rowCount = ENEMY_ROWS;
  int colCount = ENEMIES_PER_ROW;

  Point enemies[ENEMY_ROWS][ENEMIES_PER_ROW];

  double enemyFireProbability = 0.01;
  
  int level = 0;
  
protected:

  void reset() {


    bulletFired = false;
    enemiesKilled = 0;
    enemyBulletsActive = 0; 
    
    enemyBullets.clear();
    for(int i=0; i<maxEnemyBullets; i++) {
      enemyBullets.push_back({{0.5, 0.0f}, false});
    }

    enemyFireProbability = 0.01+(0.04*level);
    
    if((level+1)%2==0) {
      maxEnemyBullets += 1;
    }
    else if(level == 0) {
      maxEnemyBullets = 1;
    }
    
    for(ShieldBlock & block: shieldBlocks) {
      block.health = 3;
    }  

    for(int r=0; r < ENEMY_ROWS; r++) {
      for(int c=0; c < ENEMIES_PER_ROW; c++) {
        enemyGrid[r][c] = true;
        enemies[r][c] = {0.05f+0.13f*c, 0.03f+0.1f*r};
      }
    }

  };
};