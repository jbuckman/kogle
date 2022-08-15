#pragma once
#include "game.h"
#include <cmath>  
#include <iostream>
#include <vector>
#include <algorithm>
class Asteroids : public Game {
  public:
    
    static constexpr int boringColors[] = {75, 100, 150, 200, 225, 250};
    enum ROCK_SIZE {SMALL=1, MEDIUM=2, LARGE=3};
    
    const int SMALL_ROCK_SCORE = 100;
    const int MEDIUM_ROCK_SCORE = 50;
    const int LARGE_ROCK_SCORE = 20;
    const int UFO_SCORE = 1000;
    class Rock : public GameEntity {
      public:
        ROCK_SIZE rockSize;
        Rock(ROCK_SIZE rockSize) : GameEntity (0, 0, (int)(std::pow(2.0, rockSize)), (int)(std::pow(2.0, rockSize)), boringColors[1], false){
          this->rockSize = rockSize;
        }

        void spawn(int x, int y, float vx, float vy) {
          setX(x);
          setY(y);
          setVx(vx);
          setVy(vy);
          setIsAlive(true);
        }
    };

    class RockManager {
      public: 
        int spawnRate = 50;
        int frameTillNextSpawn = 0;
        const int MAX_LARGE=4;
        const int MAX_MED=12;
        const int MAX_SMALL=32;
        std::vector<Rock *> largeRocks;
        std::vector<Rock *> medRocks;
        std::vector<Rock *> smallRocks;
        std::vector<Rock *> rocks;

        RockManager() {

          for(int i=0; i<MAX_LARGE; i++) {
            Rock * rock = new Rock(LARGE);
            largeRocks.push_back(rock);
            rocks.push_back(rock);
          }

          for(int i=0; i<MAX_MED; i++)  {
            Rock * rock = new Rock(MEDIUM);
            medRocks.push_back(rock);
            rocks.push_back(rock);
          }

          for(int i=0; i<MAX_SMALL; i++) {
            Rock * rock = new Rock(SMALL);
            smallRocks.push_back(rock);
            rocks.push_back(rock);
          }

          reset();
        }

        void reset() {
          frameTillNextSpawn = spawnRate;
          for(Rock * rock : rocks) {
            rock->setIsAlive(false);
          }
        }
        
        Rock * getAvailableRock(ROCK_SIZE rockSize) {
          std::vector<Rock*> rockVector;

          if(rockSize == SMALL) {
            rockVector =  {smallRocks.begin(),smallRocks.end()};
          } else if(rockSize == MEDIUM) {
            rockVector =  {medRocks.begin(), medRocks.end()};
          } else if(rockSize == LARGE) {
            rockVector =  {largeRocks.begin(), largeRocks.end()};
          }

          Rock * unallocatedRock = nullptr;
          for(Rock * rock : rockVector) {
            if(!rock->getIsAlive()) {
              unallocatedRock = rock;
            }
          }
          return unallocatedRock;
        }

        void update() {
          frameTillNextSpawn--;
          if(frameTillNextSpawn<=0) {

            ROCK_SIZE rockSize = (ROCK_SIZE)uniformInteger(0,2);
            Rock * rock = getAvailableRock(rockSize);

            if(rock) {
              float randomValue = uniformFloat(0.0, 1.0);
              if(randomValue <= 0.25) {
                rock->spawn(
                  uniformInteger(rock->getWidth()+1, 32-rock->getWidth()),
                  rock->getHeight()+1,
                  uniformFloat(0.1, 0.3),
                  uniformFloat(0.1, 0.3)
                );
              } else if(randomValue <= 0.5) {
                rock->spawn(
                  uniformInteger(32, 64-rock->getWidth()),
                  rock->getHeight()+1,
                  -uniformFloat(0.1, 0.3),
                  uniformFloat(0.1, 0.3)
                );
              } else if(randomValue <= 0.75) {
                rock->spawn(
                  uniformInteger(rock->getWidth()+1, 32-rock->getWidth()),
                  64-rock->getHeight(),
                  -uniformFloat(0.1, 0.3),
                  uniformFloat(0.1, 0.3)
                );
              } else {
                rock->spawn(
                  uniformInteger(32, 64-rock->getWidth()),
                  64-rock->getHeight()-1,
                  -uniformFloat(0.1, 0.3),
                  -uniformFloat(0.1, 0.3)
                );
              }
              frameTillNextSpawn = spawnRate;
            }

          }
        }

        void kill(Rock * rock) {
          rock->setIsAlive(false);

          if(rock->rockSize > 1) {
   
            Rock * debris1 = getAvailableRock((ROCK_SIZE)((int)rock->rockSize-1));
            Rock * debris2 = getAvailableRock((ROCK_SIZE)((int)rock->rockSize-1));
            if(debris1 && debris2) {
              debris1->spawn(rock->getX(), rock->getY(), -rock->getVx(), -rock->getVy());
              debris2->spawn(rock->getX(), rock->getY(), rock->getVx(), -rock->getVy());
            }
  
          }
        }
    };

    class Bullet: public GameEntity {
      public:
        Bullet() : GameEntity (33,33,1,1, boringColors[4], false) {}
    };
    
    class Player: public GameEntity {
      private:
        int sprites[12][5][5]  = {
          {
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,1,1,1,0},
            {1,1,1,1,1},
            {1,1,1,1,1},
          },
          {
            {0,0,0,1,1},
            {0,0,1,1,1},
            {0,1,1,1,1},
            {1,1,1,1,1},
            {0,0,1,1,1},
          },
          {
            {0,0,0,0,1},
            {0,0,1,1,0},
            {0,1,1,1,0},
            {0,1,1,0,0},
            {1,0,1,0,0},
          },
          {
            {1,1,0,0,0},
            {1,1,1,0,0},
            {1,1,1,1,1},
            {1,1,1,0,0},
            {1,1,0,0,0},
          },
          {
            {0,1,0,0,0},
            {0,1,1,0,0},
            {1,1,1,1,0},
            {1,1,1,1,1},
            {1,1,1,1,1},
          },
          {
            {1,0,0,0,0},
            {0,1,1,0,0},
            {1,1,1,1,0},
            {0,0,1,1,0},
            {0,0,0,0,1},
          },
          {
            {1,1,1,1,1},
            {1,1,1,1,1},
            {0,1,1,1,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
          },
          {
            {1,1,1,0,0},
            {1,1,1,1,1},
            {1,1,1,1,0},
            {1,1,1,0,0},
            {1,1,0,0,0},
          },
          {
            {0,0,1,0,1},
            {0,0,1,1,0},
            {0,1,1,1,0},
            {0,1,1,0,0},
            {1,0,0,0,0},
          },
          {
            {0,0,0,1,1},
            {0,0,1,1,1},
            {1,1,1,1,1},
            {0,0,1,1,1},
            {0,0,0,1,1},
          },
          {
            {1,1,1,1,1},
            {1,1,1,1,1},
            {0,1,1,1,1},
            {0,0,1,1,0},
            {0,0,0,1,0},
          },
          {
            {1,0,0,0,0},
            {0,1,1,0,0},
            {0,1,1,1,1},
            {0,0,1,1,0},
            {0,0,0,0,1},
          },
      };

      public:
        Player() : GameEntity (32, 32, 5, 5, 255, true) {
          this->isSprite = true;
          this->sprite = (int*)sprites[0];
        }
        void setrotationIndex(int rotationIndex) {
          this->sprite = (int*)sprites[rotationIndex];
        }
    };

    class Ufo: public GameEntity {
      public:
        Ufo() : GameEntity (33, 32, 3, 2, boringColors[3], false) {}
    };

    class UfoBullet: public GameEntity {
      public:
        UfoBullet() : GameEntity (33, 32, 1, 1, boringColors[2], false) {}
    };

    const int MAX_PLAYER_BULLETS = 10;
    const int MAX_BULLET_FRAME_THRESHOLD = 10;
    const int TOTAL_ROTATIONS = 11;
    const int BASE_POINT_TILL_NEXT_LEVEL = 1000;

    RockManager rockManager;
    std::vector<Bullet *> playerBullets;
    Player * player;
    Ufo * ufo;
    UfoBullet * ufoBullet;
    float direction[2] = {0.0, -1.0};

    int levelCounter = 0;
    int level = 1;
    int rotationIndex = 0;

    int ufoSpawnRate[2] = {60, 100};

    int nextUfoSpawn;
    float ufoBulletProbability = 0.8;
    int bulletDelay = 0;

    const int FRAMES_TILL_NEXT_LEVEL = 300;

    Asteroids() : Game() {
      player = new Player();
      ufo = new Ufo();
      ufoBullet = new UfoBullet();
      for(int i=0; i<MAX_PLAYER_BULLETS; i++) {
        playerBullets.push_back(new Bullet());
      }
      reset();
    }

    void spawnUfo() {
      float randomNum = uniformFloat(0.0, 1.0f);
      ufo->setIsAlive(true);
      if(randomNum <= 0.25) {
        ufo->setX(uniformInteger(ufo->getWidth()+1, 32-ufo->getWidth()));
        ufo->setY(ufo->getHeight()+1);
        ufo->setVx(uniformFloat(0.1, 0.5));
        ufo->setVy(uniformFloat(0.1, 0.5));
      } else if(randomNum <= 0.5) {
        ufo->setX(uniformInteger(32, 64-ufo->getWidth()));
        ufo->setY(ufo->getHeight()+1);
        ufo->setVx(-1*uniformFloat(0.1, 0.5));
        ufo->setVy(uniformFloat(0.1, 0.5));
      } else if(randomNum <= 0.75) {
        ufo->setX(uniformInteger(ufo->getWidth()+1, 32-ufo->getWidth()));
        ufo->setY(64-ufo->getHeight()-1);
        ufo->setVx(uniformFloat(0.1, 0.5));
        ufo->setVy(-1*uniformFloat(0.1, 0.5));
      } else {
        ufo->setX(uniformInteger(32, 64-ufo->getWidth()));
        ufo->setY(64-ufo->getHeight()-1);
        ufo->setVx(-1*uniformFloat(0.1, 0.5));
        ufo->setVy(-1*uniformFloat(0.1, 0.5));
      }
    }

    void reset() {
      direction[0] = 0.0; 
      direction[1] = -1.0;

      for(int i=0; i<MAX_PLAYER_BULLETS; i++) {
        playerBullets[i]->setIsAlive(false);
      }

      rockManager.reset();

      nextUfoSpawn = randomBetween(ufoSpawnRate[0], ufoSpawnRate[1]);
      ufoBullet->setIsAlive(false);
      levelCounter = 0;
      rotationIndex = 0;
    }

    
    Bullet * getAvailablePlayerBullets() {
      Bullet * bullet = nullptr;
      for(int i=0; i<MAX_PLAYER_BULLETS; i++) {
        if(!playerBullets[i]->getIsAlive()) {
          bullet = playerBullets[i];
          break;
        }
      }
      return bullet;
    }

    void renderPixels(uint8_t * buffer) override {
      memset(buffer, 0, sizeof(uint8_t) * 64 * 64);
      player->render(buffer);
      ufo->render(buffer);
      ufoBullet->render(buffer);
      
      for(Bullet * bullet: playerBullets) {
        bullet->render(buffer);
      }

      for(Rock * rock : rockManager.rocks) {
        rock->render(buffer);
      }
    }

    void increaseDifficulty() {

      if(rockManager.spawnRate >= 25) {
        rockManager.spawnRate -= 3;
      }

      if(ufoSpawnRate[0] >= 35) {
        ufoSpawnRate[0] -= 5;
        ufoSpawnRate[1] -= 5;
      }

      if(ufoBulletProbability > 0.2) {
        ufoBulletProbability -= 0.05;
      }
    }

    void moveEntity(GameEntity * entity, bool reverse=false) {

      float vx;
      float vy;
      
      if(rotationIndex == 0) {        
        vx = 0;
        vy = -1;
      } else if(rotationIndex == 1) {
        vx = 0.5;
        vy = -0.8660254;
      } else if(rotationIndex == 2) {
        vx = 0.8660254;
        vy = -0.5;
      } else if(rotationIndex == 3) {
        vx = 1;
        vy = 0;
      } else if(rotationIndex == 4) {
        vx = 0.8660254;
        vy = 0.5;
      } else if(rotationIndex == 5) {
        vx = 0.5;
        vy = 0.8660254;
      } else if(rotationIndex == 6) {
        vx = 0;
        vy = 1;
      } else if(rotationIndex == 7) {
        vx = -0.5;
        vy = 0.8660254;
      } else if(rotationIndex == 8) {
        vx = -0.8660254;
        vy = 0.5;
      } else if(rotationIndex == 9) {
        vx = -1;
        vy = 0;
      } else if(rotationIndex == 10) {
        vx = -0.8660254;
        vy = -0.5;
      } else if(rotationIndex == 11) {
        vx = -0.5;
        vy = -0.8660254;
      }  

      if(reverse) {
        vx *= -1;
        vy *= -1;
      }
      entity->setVx(vx);
      entity->setVy(vy);
    }

    std::vector<int> step(uint8_t action) override {
      int score = 0;
      
      bool NoAction = (action == 0);
      bool Fire = (action == 1);
      bool Forward = (action == 2);
      bool Back = (action == 3);
      bool Left = (action == 4);
      bool Right = (action == 5); 
      
      if(Fire && bulletDelay <= 0) {
        Bullet * bullet = getAvailablePlayerBullets();

        if(bullet != nullptr) {

          bulletDelay = MAX_BULLET_FRAME_THRESHOLD;
          bullet->setX(player->getX()+player->getWidth()/2.0f);
          bullet->setY(player->getY()+player->getHeight()/2.0f);
          moveEntity(bullet);
          bullet->setIsAlive(true);
        }
      }

      if(Left) {
        rotationIndex = ((rotationIndex - 1) % TOTAL_ROTATIONS + TOTAL_ROTATIONS) % TOTAL_ROTATIONS;
        player->setrotationIndex(rotationIndex);
      } else if(Right) {
        rotationIndex = (rotationIndex + 1) % TOTAL_ROTATIONS;
        player->setrotationIndex(rotationIndex);        
      }

      if(Forward) {
        moveEntity(player);
      } else if(Back) {
        moveEntity(player, true);
      } else {
        player->setVx(0);
        player->setVy(0);
      }

      if(player->getY() <= 0) {
        player->setY(64-player->getHeight());
        player->setX(64-player->getX());
      } else if(player->getY() >= 64 - player->getHeight()) {
        player->setY(1);
        player->setX(64-player->getX());
      } else if(player->getX() <= 0) {
        player->setY(64-player->getY());
        player->setX(64-player->getWidth());         
      } else if(player->getX() >= 64 - player->getWidth()) {
        player->setY(64-player->getY());
        player->setX(1); 
      }
      player->update();
      
      nextUfoSpawn -= 1;

      if(nextUfoSpawn <= 0 && !ufo->getIsAlive()) {
        spawnUfo();
      }

      if(ufo->getIsAlive()) {
        if(player->collide(ufo)) {
          return {score, true};
        } else if(
            ufo->getY() <= ufo->getHeight() ||
            ufo->getX() <= ufo->getWidth()  ||
            ufo->getY() >= 64 - ufo->getHeight() ||
            ufo->getX() >= 64 - ufo->getWidth()) {

              nextUfoSpawn = randomBetween(ufoSpawnRate[0], ufoSpawnRate[1]);
              ufo->setIsAlive(false);
              ufoBullet->setIsAlive(false);
        } else {

          if(!ufoBullet->getIsAlive() && randomFloat() > ufoBulletProbability) {
            ufoBullet->setIsAlive(true);
            ufoBullet->setX(ufo->getX()+2);
            ufoBullet->setY(ufo->getY()+1);
            ufoBullet->setVx(uniformFloat(0.2, 1.0) * ((bernoulli(0.5)) ? 1 : -1));
            ufoBullet->setVy(uniformFloat(0.2, 1.0) * ((bernoulli(0.5)) ? 1 : -1));
          } else if(ufoBullet->getIsAlive()) {
              if(ufoBullet->getY() <= ufoBullet->getHeight() ||
                ufoBullet->getX() <= ufoBullet->getWidth()  ||
                ufoBullet->getY() >= 64-ufoBullet->getHeight() ||
                ufoBullet->getX() >= 64-ufoBullet->getWidth()) {

                  ufoBullet->setIsAlive(false);
              } else if(ufoBullet->collide(player)) {
                return {score, true};
              } else {
                ufoBullet->update();
              }

          }
          ufo->update();
        }
      }

      for(Bullet * bullet: playerBullets) {
        if(bullet->getIsAlive()) {
          if(bullet->getY() <= bullet->getHeight() ||
            bullet->getX() <= bullet->getWidth()  ||
            bullet->getY() >= 64-bullet->getHeight() ||
            bullet->getX() >= 64-bullet->getWidth()) {
              bullet->setIsAlive(false);
              
          } else if(ufo->getIsAlive() && ufo->collide(bullet)) {
            ufo->setIsAlive(false);
            ufoBullet->setIsAlive(false);
            bullet->setIsAlive(false);
            score += UFO_SCORE;
            nextUfoSpawn = randomBetween(ufoSpawnRate[0], ufoSpawnRate[1]); 
          } else {
            for(Rock * rock : rockManager.rocks) {
              if(rock->getIsAlive() && rock->collide(bullet)) {
              
                if(rock->rockSize == LARGE) {
                  score += LARGE_ROCK_SCORE;
                } else if(rock->rockSize==MEDIUM) {
                  score += MEDIUM_ROCK_SCORE;
                } else if(rock->rockSize==SMALL) {
                  score += LARGE_ROCK_SCORE;
                }
                rockManager.kill(rock);
                bullet->setIsAlive(false);
                break;
              }
            }

            bullet->update();
          }
        }
      }

      if(bulletDelay > 0) {
        bulletDelay--;
      }

      rockManager.update();
      for(Rock * rock : rockManager.rocks) {
        if(rock->collide(player)) {
          return {score, true};
        } else if(rock->getX() >= 64 - rock->getWidth() ||
          rock->getX() <= rock->getWidth() ||
          rock->getY() >= 64 - rock->getHeight() ||
          rock->getY() <= rock->getHeight()) {

            rockManager.kill(rock);

        } else {
          rock->update();
        }
      }

      levelCounter++;
      levelCounter += score;
      if(levelCounter >= BASE_POINT_TILL_NEXT_LEVEL * level) {
          
        levelCounter = 0; 
        level++;
        increaseDifficulty();
      }

      return {score, false};
    }

    std::vector<bool> legalActions() override {
        //      NOOP, FIRE, UP,   DOWN,   LEFT, RIGHT
        return {true, true, true, true,  true, true};
    }
};