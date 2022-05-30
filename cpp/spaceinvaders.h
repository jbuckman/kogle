#pragma once
#include "game.h"
#include <math.h>
#include <iostream>
#include <vector>

class SpaceInvaders : public Game {

  public:

    class Player : public GameEntity {
      private:
        int bitmap[3][7] =  {
          {0, 0, 1, 1, 1, 0, 0},
          {1, 1, 1, 1, 1, 1, 1},
          {1, 1, 1, 1, 1, 1, 1}
        };
      public:
        Player() : GameEntity (32, 60, 7, 3, 255, true){    
          this->isSprite = true;
          sprite = (int*)bitmap;
        }
    };

    class Enemy : public GameEntity {
      private:
        int bitmap[4][5] = {
          {1, 1, 1, 1, 1},
          {1, 0, 1, 0, 1},
          {1, 1, 1, 1, 1},
          {1, 0, 1, 0, 1}
        };
        int row;
        int col;
      public:
        Enemy(int row, int col)
        : GameEntity (6+4*row+row*3, col*5+6, 5, 4, 255, true){
          this->row = row;
          this->col = col;
          this->isSprite = true;
          sprite = (int*)bitmap;
        };

        int getCol(){return col;}
        int getRow(){return row;}
        void reset(){
          setX(6+4*row+row*3);
          setY(col*5+6);
          setIsAlive(true);
        }
    };

    Player player;

    std::vector<GameEntity> gameEntities;
    enum Direction { LEFT=0, RIGHT=1 };
    enum Event {NOTHING, ENEMY_SHOT, GAME_OVER, NEXT_LEVEL};
    static constexpr int boringColors[] = {75, 100, 150, 200, 225, 250};

    class Bullet : public GameEntity {
      public:
        Bullet() : GameEntity(0, 0, 1, 2, 255, false) {};
    };

    class ShieldBlock : public GameEntity {
      public:
        ShieldBlock(float x, float y) : 
        GameEntity(x, y, 3, 3, boringColors[3], true) {
          reset();
        }

        void reset() {
          health = 3;
          setColor(boringColors[health]);
          setIsAlive(true);
        }

        void hit() {
          setColor(boringColors[--health]);
          if(health == 0) {
            setIsAlive(false);
          }
        }
      private:
        int health = 3;
  };

  class ShieldManager {
    public:
      ShieldManager() {

        for(int i=0; i<4; i++) {
          for(int j=0; j<2; j++) {
            ShieldBlock * block = new ShieldBlock(8+3.0f*j+i*14, 52);
            shieldBlocks.push_back(block);
          }
        }
      }

      std::vector<ShieldBlock*> getShields() {
        return shieldBlocks;
      }

      void reset() {
        for (ShieldBlock * block : shieldBlocks) {
          block->reset();
        }
      }

      std::vector<ShieldBlock*> shieldBlocks;
  };


  class EnemyManager {
    private:
      static const int rowLength = 6;
      static const int columnLength = 4;
      int enemyCount;
      bool enemyMap[rowLength][columnLength];
      float chanceOfFire = 0.8;
      Direction direction;
      const int MAX_BULLETS = 4;
      int bulletsAmount;
      int difficulty = 1;

    public:
      std::vector<Enemy*> enemies;
      std::vector<Bullet*> bullets;
      std::vector<Bullet*> activeBullets;
      std::vector<Bullet*> freeBullets;
      EnemyManager() {
        for (int r = 0; r < rowLength; r++) {
          for (int c = 0; c < columnLength; c++) {
            Enemy * enemy = new Enemy(r, c);
            enemies.push_back(enemy);
            
          }
        }

        for(int i=0; i<MAX_BULLETS; i++) {
          Bullet * bullet = new Bullet();
          bullets.push_back(bullet);
        }
        reset();
      }

      void reset() {

        direction = RIGHT;
        enemyCount = rowLength*columnLength;
        for (Enemy * enemy : enemies) {
          enemy->reset();
        }
        for(int c=0; c<columnLength; c++) {
          for(int r=0; r<rowLength; r++) {
            enemyMap[r][c] = true;
          }        
        }

        bulletsAmount = std::min(MAX_BULLETS, difficulty);

        for (Bullet * bullet : bullets) {
          bullet->setIsAlive(false);
        }

        freeBullets.clear();

        for(int i=0; i<bulletsAmount; i++) {
          Bullet * bullet = bullets[i];
          freeBullets.push_back(bullet);
        }

        activeBullets.clear();
      }


      Event update(GameEntity & playerBullet) {
        bool descend = false;
        Event event = NOTHING;
        if(direction == RIGHT) {
          for(int r=rowLength-1; r>=0; r--) {
            for(int c=columnLength-1; c>=0; c--) {
              updateEnemy(playerBullet, c, r, descend, event);

            }        
          }
        } else {
          for(int r=0; r<rowLength; r++) {
            for(int c=0; c<columnLength; c++) {
              updateEnemy(playerBullet, c, r, descend, event);

            }        
          }
        }
      
        if(!freeBullets.empty() && chanceOfFire < uniform(0,1)) {
          int row = uniformInteger(0, rowLength);
          int col = uniformInteger(0, columnLength);
          if(enemyMap[row][col]) {
            Enemy * enemy = enemies[col+row*columnLength];
            Bullet * bullet = freeBullets.back();
            freeBullets.pop_back();
            bullet->setX(enemy->getX()+enemy->getWidth()/2);
            bullet->setY(enemy->getY()+enemy->getHeight());
            bullet->setIsAlive(true);
            bullet->setVy(1);
            bullet->update();
            activeBullets.push_back(bullet);
          }
        }
       
        return event;
      }

      void updateEnemy(GameEntity & playerBullet, int column, int row, bool & descend, Event & event) {
        Enemy * enemy = enemies[column+row*columnLength];
        if(enemy->getIsAlive()){
          enemy->setVy(0);
          float vx = 0; 
          float vy = 0;
          
          if(playerBullet.getIsAlive() && enemy->collide(playerBullet)) {
            enemy->setIsAlive(false);
            playerBullet.setIsAlive(false);
            enemyMap[row][column] = false;
            enemyCount--;

            if(enemyCount==0) {
              event = NEXT_LEVEL;
            } else {
              event = ENEMY_SHOT;
            }
            
            return;
          }
    

          if(!descend) {
            if(enemy->getX() >= 60 || enemy->getX() <= 0) {
              descend = true;
              direction =(Direction)(((int)direction) ^ 1);
            } 
          }

          if(descend) {
            vx = (direction == LEFT) ? -1 : 1;
            vy = 1;
          }
          vx = (direction == LEFT) ? -1 : 1;
          enemy->setVx(vx);
          enemy->setVy(vy);
          enemy->update();

          if(enemy->getY() >= 50) {
            event = GAME_OVER;
          }
        }
      }

  };

  Bullet playerBullet;
  EnemyManager enemyManager = EnemyManager();
  ShieldManager shieldManager = ShieldManager();

  SpaceInvaders() : Game() {}

  void nextLevel() {
    std::cout << "hello"; 
    enemyManager.reset();
  }
  
  std::vector<bool> legalActions() override {
    //      NOOP, FIRE, UP,   DOWN,   LEFT, RIGHT
    return {true, true, false, false,  true, true};
  }

  void renderPixels(uint8_t * buffer) override {
    memset(buffer, 0, sizeof(uint8_t) * 64 * 64);
    
    player.render(buffer);
    playerBullet.render(buffer);
    for(Enemy * enemy : enemyManager.enemies) {
      enemy->render(buffer);
    }
    
    for(Bullet * bullet : enemyManager.bullets) {
      bullet->render(buffer);
    }

    for(ShieldBlock * shield : shieldManager.getShields()) {
      shield->render(buffer);
    }
    
  }

  std::vector<int> step(uint8_t action) override {
    bool Fire = (action == 1);
    bool Up = (action == 2);
    bool Down = (action == 3);
    bool Left = (action == 4);
    bool Right = (action == 5); 
    int score = 0;
    
    if(Left && player.getX() > 0) {
      player.setVx(-1);
      player.update();

    } else if(Right && player.getX() < 60) {
      player.setVx(1);
      player.update();

    } else if(Fire && !playerBullet.getIsAlive()) {
      playerBullet.setY(player.getY()-2);
      playerBullet.setX(player.getX()+player.getWidth()/2);
      playerBullet.setIsAlive(true);
    }
 
    Event event = enemyManager.update(playerBullet);

    if(event == GAME_OVER){
      return {score, true};
    } else if(event == ENEMY_SHOT){
      score += 10;
    } else if(event == NEXT_LEVEL){
      score += 10;
      nextLevel();
      return {score, false};
    }
 
    for (int i = 0; i < enemyManager.activeBullets.size(); i++) {
      bool despawn = false;
      Bullet * enemyBullet = enemyManager.activeBullets[i];
      for(ShieldBlock * shieldPtr: shieldManager.getShields()) {
        ShieldBlock shield = *(shieldPtr);
        if(shieldPtr->getIsAlive() && playerBullet.collide(shield)){
          shieldPtr->hit();
          playerBullet.setIsAlive(false);
        }
        if(shield.getIsAlive() && enemyBullet->collide(shield)) {
          shieldPtr->hit();
          despawn = true;
        }
      }

      if(!despawn && enemyBullet->collide(player)) {
        return {0, true};
      }

      if(!despawn && enemyBullet->getY() >= 64) {
        despawn = true;
      }

      if(despawn) {
        enemyBullet->setIsAlive(false);
        enemyManager.freeBullets.push_back(enemyBullet);
        enemyManager.activeBullets.erase(enemyManager.activeBullets.begin()+i);
      } else {
        enemyBullet->update();
      }
    }

    if(playerBullet.getIsAlive()) {
      playerBullet.setVy(-2);
      playerBullet.update();
      
      if(playerBullet.getY() <= 0) {
        playerBullet.setIsAlive(false);
      }
    }

    return {score, false};

  }
};