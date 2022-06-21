/*
#pragma once
#include "game.h"
#include <math.h>
#include <iostream>

class Seaquest : public Game {

public:
  Seaquest() : Game() {}

  std::vector<bool> legalActions() override {
    //      NOOP, FIRE, UP,   DOWN,   LEFT, RIGHT
    return {true, true, true,   true, true,  true};
  }

  void renderPixels(uint8_t * buffer) override { 
    memset(buffer, 0, sizeof(uint8_t) * 64 * 64);
    memset(buffer, 30, sizeof(uint8_t) * 64 * 10);
    drawSprite(buffer, player, 150);

    if(playerBullet.fired) {
      drawRect(buffer, playerBullet.point.x*64.0, playerBullet.point.y*64.0, 64.0*BULLET_WIDTH, 64.0*BULLET_HEIGHT, 255);
    }

    if(spawnedDivers > 0) {
      for(GameEntity &diver: divers) {
        if(diver.isAlive) {
          drawSprite(buffer, diver, 150);
        }
      }
    }

    for(int i=0; i<MAX_ENEMY_SPAWNED; i++) {
      if(enemies[i].entity.isAlive) {
        drawSprite(buffer, enemies[i].entity, 255);
      }
      if(enemyBullets[i].fired) {    
        drawRect(buffer, enemyBullets[i].point.x*64.0, enemyBullets[i].point.y*64.0, 64.0*BULLET_WIDTH, 64.0*BULLET_HEIGHT, 255); 
      }
    }     
    
    if(oxygenLevel > 0) {
      drawRect(buffer, 10.0f, 60.0f, 4.0f*oxygenLevel/100, 2.0f, 255);
    }

    for(int i=0; i<diversSaved; i++) {
      drawVector(buffer, diverSprite, 0.25f+0.09375*i, 0.0f, 150);
    }
  }

  std::vector<int> step(uint8_t action) override {

    int score = 0;

    bool Up = (action == 2) | (action == 2 + 4);
    bool Down = (action == 3) | (action == 3 + 4);
    bool Left = (action == 4) | (action == 4 + 4);
    bool Right = (action == 5) | (action == 5 + 4);
    bool Fire = (action == 1);

    if (Right) {
      player.rect.x += PLAYER_X_SPEED;
      player.direction = RIGHT;
    } else if (Left) {
      player.rect.x -= PLAYER_X_SPEED;
      player.direction = LEFT;
    } else if (Up) {
      player.rect.y  -= PLAYER_Y_SPEED;
    } else if (Down) {
      player.rect.y += PLAYER_Y_SPEED;
    } else if(Fire && !playerBullet.fired) {
      
      // Bullet will go in the direction player is going.
      if(player.direction == LEFT) {

        playerBullet = {
          {(player.rect.x-player.rect.width-1.0f/64.0f), player.rect.y+player.rect.height/4.0f}, 
          LEFT, true
        };

      } else {
        
        playerBullet = {
          {(player.rect.x+player.rect.width+2.0f/64.0f), player.rect.y+player.rect.height/4.0f}, 
          RIGHT, true
        };
      }  
    }

    // move bullet if fired
    if(playerBullet.fired) {
      
      if(playerBullet.direction == LEFT) {
        playerBullet.point.x -= PLAYER_BULLET_SPEED;
      } else {
        playerBullet.point.x += PLAYER_BULLET_SPEED;
      }

      if(playerBullet.point.x <= 0.0f || playerBullet.point.x >= 1.0f) {
        playerBullet.fired = false;
      }
    }

    // player cannot move outside of range
    player.rect.x = std::max(0.f, std::min(1.0f, player.rect.x));
    player.rect.y = std::max(MIN_PLAYER_Y, std::min(MAX_PLAYER_Y, player.rect.y));

    // Keep track if player is still up refilling oxygen from last turn
    // Player has reached smallest Y coordinate on game board
    if(player.rect.y == MIN_PLAYER_Y && prevPlayerY != player.rect.y) {
      if(diversSaved == MAX_DIVERS_SAVED) {
        score += 200;
        diversSaved = 0;
        oxygenLevel = FULL_OXYGEN;
      } else if(diversSaved > 0) {
        score += 20;
        diversSaved--;
        oxygenLevel = FULL_OXYGEN;
      } else {
        return {score, true};
      }
    } else if(player.rect.y != MIN_PLAYER_Y || prevPlayerY != player.rect.y) {
      oxygenLevel--;
    } 

    if(oxygenLevel <= 0) {
      return {score, true};   
    }
    
    prevPlayerY = player.rect.y;
    
    if(diverFrameCount >= nextDiverFrameCount && spawnedDivers == 0 && diversSaved < MAX_DIVERS_SAVED) {
      diverFrameCount = 0;
      nextDiverFrameCount = randomBetween(60, 100);

      switch(diceRoll()) {
        case 5:
        case 6:
          spawnDivers();
          break;
        case 4:
          spawnDivers(2);
          break;
        default:
          break;
      }
    } else if(spawnedDivers > 0) {
      for(GameEntity &diver: divers) {
        if(diver.isAlive) {
          bool diverDespawned = false;
          if(collide(player.rect, diver.rect)) {
            score += 10;
            diversSaved++;
            diverDespawned = true;

          } else if(diver.direction == LEFT) {

            diver.rect.x -= DIVER_SPEED;
            if(diver.rect.x <= 0.0f) {
              diverDespawned = true;
            }

          } else {
            diver.rect.x += DIVER_SPEED;

            if(diver.rect.x >= 1.0f) {
              diverDespawned = true;
            }
          }

          if(diverDespawned) {
            spawnedDivers--;
            diver.isAlive = false;
          }
        }

      }     
    } else {
      diverFrameCount++;
    }

    if(enemyFrameCount >= nextEnemyFrameCount && enemiesSpawned == 0) {
      enemyFrameCount = 0;
      nextEnemyFrameCount = randomBetween(50, 100);

      switch(diceRoll()) {
        case 6:
          spawnEnemies(4);
          break;
        case 5:
        case 4:
          spawnEnemies(2);
          break;
        case 3:
        case 2:
          spawnEnemies(1);
          break;
        default:
          break;
      }
    }

    enemyFrameCount++;

    for(int i=0; i<MAX_ENEMY_SPAWNED; i++) {
      bool enemyDespawned = false;
      if(enemies[i].entity.isAlive) {
          
        if(collide(enemies[i].entity.rect, player.rect)) {
          return {score, true}; 
        } else if(collide(enemies[i].entity.rect, {playerBullet.point.x, playerBullet.point.y, BULLET_WIDTH, BULLET_HEIGHT})) {
          playerBullet.fired = false;
          enemyDespawned = true;
          score += 20;
        } else if(enemies[i].entity.direction == LEFT) {

          enemies[i].entity.rect.x -= ENEMY_SPEED;
  
          if(enemies[i].entity.rect.x <= 0.0f) {
            enemyDespawned = true;
          } else if(enemies[i].canFireBullets && !enemyBullets[i].fired) {
            
            // you are in enemy crosshairs 
            if(enemies[i].entity.rect.y - 0.04f < player.rect.y
                && enemies[i].entity.rect.y + 0.04f > player.rect.y 
                && enemies[i].entity.rect.x > player.rect.x
            ) {

              enemyBullets[i] = {
              {
                enemies[i].entity.rect.x-enemies[i].entity.rect.width-1.0f/64.0f, 
                enemies[i].entity.rect.y+enemies[i].entity.rect.height/4.0f
              }, LEFT, true};
          
            }   
          } 
   
        } else if(enemies[i].entity.direction == RIGHT) {
          
          enemies[i].entity.rect.x += ENEMY_SPEED;

          if(enemies[i].entity.rect.x >= 1.0f) {
            enemyDespawned = true;
          } else if(enemies[i].canFireBullets && !enemyBullets[i].fired) { 

            // you are in enemy crosshairs 
            if(enemies[i].entity.rect.y - 0.04f < player.rect.y
              && enemies[i].entity.rect.y + 0.04f > player.rect.y 
              && enemies[i].entity.rect.x < player.rect.x
            ) {
                enemyBullets[i] = {
                {
                  enemies[i].entity.rect.x+enemies[i].entity.rect.width+1.0f/64.0f, 
                  enemies[i].entity.rect.y+enemies[i].entity.rect.height/4.0f
                }, RIGHT, true};            
            } 
          } 
        }

        if(enemyDespawned) {
          enemiesSpawned--;
          enemies[i].entity.isAlive = false;           
        }
      }
    }

    for(Bullet & enemyBullet: enemyBullets) {
      if(enemyBullet.fired) {

        if(collide(player.rect, {enemyBullet.point.x, enemyBullet.point.y, BULLET_WIDTH, BULLET_HEIGHT})) {
          return {score, true};
        } else if(enemyBullet.direction == LEFT) {

          enemyBullet.point.x -= ENEMY_BULLET_SPEED;
          if(enemyBullet.point.x <= 0.0f) {
            enemyBullet.fired = false;
          }

        } else if(enemyBullet.direction == RIGHT) {

          enemyBullet.point.x += ENEMY_BULLET_SPEED;
          if(enemyBullet.point.x >= 1.0f) {
            enemyBullet.fired = false;
          }
        }
      }
    }
 
    return {score, false};  
  }

private:

  std::vector<std::vector<bool>> submarineSprite = {
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1}
  };

  std::vector<std::vector<bool>> sharkSprite = {
    {0, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 1}    
  };
  
  GameEntity player = {0.5, 0.7, submarineSprite, true, LEFT};

  const float PLAYER_X_SPEED = 0.005f;
  const float PLAYER_Y_SPEED = 0.002f;

  float MIN_PLAYER_Y = 0.1f;
  float MAX_PLAYER_Y = 0.9f;
  float prevPlayerY = player.rect.y;

  struct Bullet {
    Point point;
    Direction direction;
    bool fired;
  };

  Bullet playerBullet = {0.0f, 0.0f, LEFT, false};
  const float PLAYER_BULLET_SPEED = 0.05f;

  const float BULLET_WIDTH = 3.0f/64.0f;
  const float BULLET_HEIGHT = 1.5f/64.0f;
  
  const int FULL_OXYGEN = 1000;
  int oxygenLevel = FULL_OXYGEN;

  std::vector<std::vector<bool>> diverSprite = {
    {0, 1, 0, 0},
    {1, 1, 1, 1},
    {0, 1, 1, 0},
    {0, 0, 1, 1}
  };

  GameEntity divers[2] = {
    {-0.1, -0.1, diverSprite, false, LEFT}, 
    {-0.1, -0.1, diverSprite, false, LEFT}
  };
  
  int nextDiverFrameCount = 50;
  int diverFrameCount = 0;
  
  int diversSaved = 0;
  const int MAX_DIVERS_SAVED = 6;

  int spawnedDivers = 0;
  const float DIVER_SPEED = 0.003f; 

  void spawnDivers(int count=1) {
    
    if(count > 1 && diversSaved == MAX_DIVERS_SAVED - 1) {
      count = 1;
    }

    for(int i=0; i<count; i++) {
      divers[i].rect.y = uniform(0.2, 0.7);
      divers[i].direction = (bernoulli(0.5)) ? LEFT : RIGHT;
      divers[i].rect.x = (divers[i].direction==LEFT) ? 1.0f : 0.0f;
      divers[i].isAlive = true;
      spawnedDivers++;
    }
  }

  int nextEnemyFrameCount = 30;
  int enemyFrameCount = 0;
  int enemiesSpawned = 0;
  const int MAX_ENEMY_SPAWNED = 4;
  const float ENEMY_SPEED = 0.002f;

  struct Enemy {
    GameEntity entity;
    bool canFireBullets;
  };

  Enemy enemies[4] = {
    {{-0.1, -0.1, sharkSprite, false, LEFT},false},
    {{-0.1, -0.1, sharkSprite, false, LEFT},false},
    {{-0.1, -0.1, sharkSprite, false, LEFT},false},
    {{-0.1, -0.1, sharkSprite, false, LEFT},false},
  };

  Bullet enemyBullets[4] = {
    {0.0f, 0.0f, LEFT, false},
    {0.0f, 0.0f, LEFT, false},
    {0.0f, 0.0f, LEFT, false},
    {0.0f, 0.0f, LEFT, false}
  };

  const float ENEMY_BULLET_SPEED = 0.003f;

  void spawnEnemies(int count=1) {

    for(int i=0; i<count; i++) {
      
      enemies[i].entity.rect.y = uniform(0.2, 0.7);
      enemies[i].entity.direction = (bernoulli(0.5)) ? LEFT : RIGHT;
      enemies[i].entity.rect.x = (enemies[i].entity.direction==LEFT) ? 1.0f : 0.0f;
      enemies[i].entity.isAlive = true;
      enemies[i].canFireBullets = bernoulli(0.5);
      enemies[i].entity.sprite = (enemies[i].canFireBullets) ? submarineSprite : sharkSprite;
      enemiesSpawned++;
    }
  }
};
*/