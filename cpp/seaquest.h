#pragma once
#include "game.h"
#include <math.h>
#include <iostream>
#include <vector>

class Seaquest : public Game {

  public:
    static constexpr int boringColors[] = {75, 100, 150, 200, 225, 250};
    enum GAME_EVENT { LOSE=0, RESCUE=1, NEXT_LEVEL=2, ENEMY_SHOT=3, NOTHING=4 };
    enum Direction { LEFT=0, RIGHT=1 };
    class Player : public GameEntity {
      private:
        int leftBitmap[4][8] =  {
          {0, 0, 1, 1, 0, 0, 0, 0},
          {0, 0, 1, 1, 1, 0, 0, 0},
          {1, 1, 1, 1, 1, 1, 1, 0},
          {1, 1, 1, 1, 1, 1, 1, 1}
        };
        int rightBitmap[4][8] =  {
          {0, 0, 0, 0, 1, 1, 0, 0},
          {0, 0, 0, 1, 1, 1, 0, 0},
          {0, 1, 1, 1, 1, 1, 1, 1},
          {1, 1, 1, 1, 1, 1, 1, 1}
        };
      public:
        Direction direction;
        Player() : GameEntity (32, 32, 8, 4, 255, true){    
          this->isSprite = true;
          sprite = (int*)leftBitmap;
          direction = LEFT;
        }

        void setDirection(Direction direction) {
          this->direction = direction;
          if(this->direction == LEFT) {
            sprite = (int*)leftBitmap;
          } else {
            sprite = (int*)rightBitmap;
          }
        }

        void update() {
          if(this->direction == LEFT) {
            sprite = (int*)leftBitmap;
          } else {
            sprite = (int*)rightBitmap;
          }
          GameEntity::update();
        }
    };

    class Bullet : public GameEntity {
      public:
      Bullet() : GameEntity (32, 3, 2, 1, 255, false) {}
    };
    enum ENEMY_TYPE { SHARK=0, SUBMARINE=1, BULLET=2};
    class Enemy : public GameEntity {
      protected:
        int * leftSprite;
        int * rightSprite;
      public:
        Direction direction;
        bool canFire = false;
        ENEMY_TYPE enemyType;
        float speed;
        bool hasFired = false;
        Enemy(float _width, float _height, uint8_t _color) : GameEntity (33, 33, _width, _height, boringColors[3], false){    

          direction = LEFT;
        }

        void update() {
          if(this->getIsAlive()) {
            if(this->direction==LEFT) {
              this->setVx(-this->speed);
            } else {
              this->setVx(this->speed);
            }
            GameEntity::update();
          }
        }

        void spawn(float x, float y, float speed, Direction direction) {
          this->setIsAlive(true);
          this->setX(x);
          this->setY(y);
          this->speed = speed;
          this->setDirection(direction);
        }

        void setDirection(Direction direction) {
          this->direction = direction;
          if(this->direction == LEFT) {
            sprite = leftSprite;
          } else {
            sprite = rightSprite;
          }
        }
    };
    
    class EnemyBullet : public Enemy {
      public:
        EnemyBullet() : Enemy(2, 1, boringColors[2]){ 
          this->enemyType = BULLET;
        }

        void spawn(float x, float y, float speed, Direction direction) {
          this->setIsAlive(true);
          this->setY(y);
          this->setX(x);
          this->direction = direction;
          this->speed = speed;
          if(this->direction == LEFT) {
            this->setVx(-speed);
          } else {
            this->setVx(speed);
          }
          
        }
    };

    class Shark : public Enemy {
      private:
        int leftBitmap[4][8] =  {
          {0, 1, 1, 1, 0, 0, 0, 1},
          {1, 1, 1, 1, 1, 0, 1, 1},
          {1, 1, 1, 1, 1, 1, 1, 1},
          {0, 1, 1, 1, 1, 1, 1, 1}   
        };
        int rightBitmap[4][8] =  {
          {1, 0, 0, 0, 1, 1, 1, 0},
          {1, 1, 0, 1, 1, 1, 1, 1},
          {1, 1, 1, 1, 1, 1, 1, 1},
          {1, 1, 1, 1, 1, 1, 1, 0}
        };
      public: 
        bool flicker = false;
        Shark() : Enemy(8, 4, boringColors[2]) { 
          this->enemyType = SHARK;
          this->isSprite = true;
          leftSprite = (int*) leftBitmap;
          rightSprite = (int*) rightBitmap;
          sprite = leftSprite;
        }

        void update() {
          if(this->getIsAlive()) {
            if(flicker) {
              this->setVy(1);
            } else {
              this->setVy(-1);
            }
            flicker = !flicker;
            if(this->direction==LEFT) {
              this->setVx(-this->speed);
              sprite = (int*)leftBitmap;
            } else {
              this->setVx(this->speed);
              sprite = (int*)rightBitmap;
            }
            GameEntity::update();
          }
        } 
    };

    class Submarine : public Enemy {
      public:
        int leftBitmap[4][8] =  {
          {0, 0, 1, 1, 0, 0, 0, 0},
          {0, 0, 1, 1, 1, 0, 0, 0},
          {1, 1, 1, 1, 1, 1, 1, 0},
          {1, 1, 1, 1, 1, 1, 1, 1}
        };
        int rightBitmap[4][8] =  {
          {0, 0, 0, 0, 1, 1, 0, 0},
          {0, 0, 0, 1, 1, 1, 0, 0},
          {0, 1, 1, 1, 1, 1, 1, 1},
          {1, 1, 1, 1, 1, 1, 1, 1}
        };
        Submarine() : Enemy(8, 4, boringColors[3]) { 
          this->enemyType = SUBMARINE;
          this->canFire = true;
          this->isSprite = true;
          leftSprite = (int*) leftBitmap;
          rightSprite = (int*) rightBitmap;
          sprite = leftSprite;
        }
        void update() {
          if(this->getIsAlive()) {

            if(this->direction==LEFT) {
              this->setVx(-1);
              sprite = (int*)leftBitmap;
            } else {
              this->setVx(1);
              sprite = (int*)rightBitmap;
            }
            GameEntity::update();
          }
        } 

    };

    static const int MAX_SHARKS = 10;
    static const int MAX_SUBS = 6;
    static const int MAX_BULLETS = 6;

    class EnemyManager {
      public:
        std::vector<EnemyBullet*> bullets;
        std::vector<Shark*> sharks;
        std::vector<Submarine*> submarines;

        int sharksAllocated = 0;
        int bulletsAllocated = 0;
        int submarinesAllocated = 0;
        int frameNextSharkBounds[2] =  {20, 40};
        int frameNextSubBounds[2] = {100, 150};
        int frameTillNextSharkSpawn = 5;
        int frameTillNextSubSpawn = 20;
        int difficulty = 0;
        EnemyManager() {
          for (int i = 0; i < MAX_SHARKS; i++) {
            sharks.push_back(new Shark());
          }
          for (int i = 0; i < MAX_SUBS; i++) {
            submarines.push_back(new Submarine());
          }
          for (int i = 0; i < MAX_BULLETS; i++) {
            bullets.push_back(new EnemyBullet());
          }
        }

        Enemy * getAvailableEnemy(ENEMY_TYPE enemyType) {
          std::vector<Enemy*> enemyVector;
          if(enemyType == SHARK) {
            enemyVector = {sharks.begin(),sharks.end()};
          } else if(enemyType == SUBMARINE) {
            enemyVector = {submarines.begin(),submarines.end()};
          } else if(enemyType == BULLET) {
            enemyVector =  {bullets.begin(),bullets.end()};
          }
          Enemy * unallocatedEnemy = NULL;
          for(Enemy * enemy : enemyVector) {
            if(!enemy->getIsAlive()) {
              unallocatedEnemy = enemy;
            }
          }
          return unallocatedEnemy;
        }

        void spawn(ENEMY_TYPE enemyType) {
          int amount = uniform(1,2);
          Enemy * spawnedEnemy[2];
          int pos[2] = {0, 0};
          if(enemyType==SHARK && sharksAllocated + amount <= MAX_SHARKS) {
            
            for(int i=0; i<amount; i++) {
              spawnedEnemy[i] = getAvailableEnemy(SHARK);
            }
            
          } else if(enemyType==SUBMARINE && submarinesAllocated + amount < MAX_SUBS) {

            for(int i=0; i<amount; i++) {
              spawnedEnemy[i] = getAvailableEnemy(SUBMARINE);
              spawnedEnemy[i]->hasFired = false;
            }

          } else {
            return;
          }

          if(amount == 1) {
            pos[0] = uniform(7,56);
          } else {
            pos[0] = uniform(7,30);
            pos[1] = uniform(35,56);
          }

          for(int i=0; i<amount; i++) {
            Direction direction = (randomFloat() > 0.5) ? LEFT : RIGHT;
            float x = (direction==LEFT) ? 70 : -10;
            float enemySpeed = 1.0;

            if(enemyType == SHARK) {
              if(difficulty == 1) {
                enemySpeed += (randomFloat() > 0.8) ? 1 : 0;
              } else if(difficulty==2) {
                enemySpeed += (randomFloat() > 0.7) ? 1 : 0;
              } else if(difficulty>=3) {
                enemySpeed += (randomFloat() > 0.5) ? 1 : 0;
              }

              spawnedEnemy[i]->spawn(x, pos[i], enemySpeed, direction);
            } else if(enemyType == SUBMARINE) {
              spawnedEnemy[i]->spawn(x, pos[i], enemySpeed, direction);
            }
          }   
        }
        void nextLevel() {
          difficulty += 1;
          if(difficulty >= 2) {
            frameNextSharkBounds[0] = std::max(10, frameNextSharkBounds[0]-2);
            frameNextSharkBounds[1] = std::max(30, frameNextSharkBounds[1]-2);
            frameNextSubBounds[0] = std::max(50, frameNextSubBounds[0]-10);
            frameNextSubBounds[1] = std::max(100, frameNextSubBounds[1]-10);
          }
          for (int i = 0; i < MAX_SHARKS; i++) {
            sharks[i]->setIsAlive(false);
          }
          for (int i = 0; i < MAX_SUBS; i++) {
            submarines[i]->setIsAlive(false);
          }
          for (int i = 0; i < MAX_BULLETS; i++) {
            bullets[i]->setIsAlive(false);
          }
        }

        GAME_EVENT update(Player * player, Bullet * bullet) {
          GAME_EVENT event = NOTHING;
          frameTillNextSharkSpawn--;
          frameTillNextSubSpawn--;

          if(frameTillNextSharkSpawn <= 0) {
            spawn(SHARK);
            frameTillNextSharkSpawn = uniform(frameNextSharkBounds[0], frameNextSharkBounds[1]);
          }
          if(frameTillNextSubSpawn <= 0) {
            spawn(SUBMARINE);
            frameTillNextSubSpawn  = uniform(frameNextSubBounds[0], frameNextSubBounds[1]);
          }


          for(int i=0; i<MAX_SHARKS; i++) {
            updateEnemy(event, sharks[i], player, bullet);
          }
          for(int i=0; i<MAX_SUBS; i++) {
            updateEnemy(event, submarines[i], player, bullet);
          }
          for(int i=0; i<MAX_BULLETS; i++) {
            updateEnemy(event, bullets[i], player, bullet);
          }
   

          return event;
        }

        void  updateEnemy(GAME_EVENT & event, Enemy * enemy, Player * player, Bullet * bullet){

          if(enemy->getIsAlive()){
            bool enemyDespawned = false;

            if((enemy->getX() <= 0 && enemy->direction == LEFT) ||
              (enemy->getX() >= 64.0-enemy->getWidth() && enemy->direction == RIGHT)) {
                enemyDespawned = true;
            } else if(enemy->collide(player)) {
              event = LOSE; 
            } else if(enemy->collide(bullet) && enemy->enemyType != BULLET) {
              enemyDespawned = true;
              bullet->setIsAlive(false);
              event = ENEMY_SHOT;
            } else {
              if(enemy->enemyType == SUBMARINE) {
              
                if(!(enemy->hasFired) && (enemy->getY() - 1 <= player->getY()) &&
                  (enemy->getY()+enemy->getHeight()+1 >= player->getY()) && 
                  (enemy->getX() >= 0 and enemy->getX() <= 64)) {

                    if((enemy->direction == RIGHT && enemy->getX() < player->getX()) ||
                    (enemy->direction == LEFT && enemy->getX() > player->getX())) {
                      enemy->hasFired = true;
                      fireBullet(enemy);
                    }
                }
              }
            }

            if(enemyDespawned) {
              enemy->setIsAlive(false);
              enemy->hasFired = false;
              if(enemy->enemyType == BULLET) {
                bulletsAllocated--;
              }
            } else {
              enemy->update();
            }
   
          }
          
        }

        void fireBullet(Enemy * enemy) {
          if(bulletsAllocated < MAX_BULLETS) {
            
            EnemyBullet * enemyBullet = (EnemyBullet*)getAvailableEnemy(BULLET);
            float y = enemy->getY()+enemy->getHeight()/2.0;
            float x = (enemy->direction==LEFT) ? enemy->getX() : enemy->getX()+enemy->getWidth();
            float speed = 2.0;
            if(difficulty==1) {
              speed += (randomFloat() > 0.8) ? 1 : 0;
            } else if(difficulty==2) {
              speed += (randomFloat() > 0.7) ? 1 : 0;
            } else if(difficulty==3) {
              speed += (randomFloat() > 0.5) ? 1 : 0;
            }

            enemyBullet->spawn(x, y, speed, enemy->direction);
            bulletsAllocated++;
          }
        }
    };

    class Diver : public GameEntity {
      private:
        int leftBitmap[4][4] =  {
          {0, 1, 0, 0},
          {1, 1, 1, 1},
          {0, 1, 1, 0},
          {0, 0, 1, 1}
        };
        int rightBitmap[4][4] =  {
          {0, 0, 1, 0},
          {1, 1, 1, 1},
          {0, 1, 1, 0},
          {1, 1, 0, 0}
        };
      public:
        Direction direction;
        Diver() : GameEntity (32, 60, 4, 4, 255, false){    
          this->isSprite = true;
          sprite = (int*)leftBitmap;
          direction = LEFT;
        }

        void setDirection(Direction direction) {
          this->direction = direction;
          if(this->direction == LEFT) {
            sprite = (int*)leftBitmap;
          } else {
            sprite = (int*)rightBitmap;
          }
        }
    };
    static const int MAX_DIVERS_SPAWNED = 2;
    static const int MAX_DIVERS_SAVED = 6;
    static constexpr float DIVER_SPEED = 0.5;

    class DiverManager { 
      public:
        int frameNextDiverBounds[2] = {20, 40};
        int frameTillNextSpawn = 0;
        std::vector<Diver*> divers;
        std::vector<Diver*> diversSaved;
        int diversSpawned = 0;
        int diversSavedCount = 0;

        DiverManager() {
          for (int i = 0; i < MAX_DIVERS_SPAWNED; i++) {
            divers.push_back(new Diver());
          }
          for (int i = 0; i < MAX_DIVERS_SAVED; i++) {
            Diver * diver = new Diver();
            diver->setY(0);
            diver->setX(12+((diver->getWidth()+1)*i));
            diversSaved.push_back(diver);
          }
          reset();
        }

        void reset() {
          frameTillNextSpawn = uniform(frameNextDiverBounds[0], frameNextDiverBounds[1]);
          for (int i = 0; i < MAX_DIVERS_SPAWNED; i++) {
            divers[i]->setIsAlive(false);
          }
          for (int i = 0; i < MAX_DIVERS_SAVED; i++) {
            diversSaved[i]->setIsAlive(false);
          }   
          diversSavedCount = 0;  
            diversSpawned = 0; 
          }

        GAME_EVENT retrieveOxygen() {
          switch(diversSavedCount) {
            case 0:
              return LOSE;
            case MAX_DIVERS_SAVED:
              reset();
              return NEXT_LEVEL;
            default:
              diversSavedCount--;
              diversSaved[diversSavedCount]->setIsAlive(false);
              return RESCUE;
          }
        }

        void update(Player * player) {
          frameTillNextSpawn--;

          if(frameTillNextSpawn <= 0 && diversSpawned==0 && diversSavedCount < MAX_DIVERS_SAVED) {

            if(diversSavedCount < MAX_DIVERS_SAVED-1) {
              diversSpawned = (randomFloat() > 0.5) ? 1 : 2;
            } else {
              diversSpawned = 1;
            }
            for(int i=0; i<diversSpawned; i++) {
              divers[i]->setIsAlive(true);
              divers[i]->setY((float)uniform(6,55));

              float left = randomFloat();
              if(left < 0.5) {
                divers[i]->setX(0);
                divers[i]->setVx(0.5);
              } else {
                divers[i]->setX(62);
                divers[i]->setVx(-0.5);
              }
            }
          }
          if(diversSpawned > 0) {
            for(Diver * diver : divers) {
              if(diver->getIsAlive()) {
                if((diver->getVx() == 0.5 && diver->getX() >=63) &&
                (diver->getVx() == -0.5 && diver->getX() <= 1)) {
                  diver->setIsAlive(false);
                  diversSpawned --;
                } else if(player->collide(diver)) {
                  diver->setIsAlive(false);
                  diversSaved[diversSavedCount++]->setIsAlive(true);
                  if(diversSpawned > 0) {
                    diversSpawned --;
                  } else {
                    frameTillNextSpawn = uniform(frameNextDiverBounds[0], frameNextDiverBounds[1]);
                    break;
                  }
                } else {
                  diver->update();
                }
              }
            }

            if(diversSpawned == 0) {
              frameTillNextSpawn = uniform(frameNextDiverBounds[0], frameNextDiverBounds[1]);
            }
          }
        }
    };

    int pointsFromEnemyShot = 20;
    int pointsFromDiverSaved = 50;

    static const int MIN_PLAYER_X = 0;
    static const int MAX_PLAYER_X = 60;
    static const int MIN_PLAYER_Y = 3;
    static const int MAX_PLAYER_Y = 57;

    class Air : public GameEntity {
      public:
        Air() : GameEntity (0, 0, 64, 6, boringColors[1], true){}
    };

    class OxygenBar : public GameEntity {
      public:
        OxygenBar() : GameEntity (6, 60, 50, 2, boringColors[1], true){}
    };

    Air * air = NULL;
    OxygenBar * oxygenBar = NULL; 
    Player * player = NULL;
    Bullet * bullet = NULL;
    DiverManager * diverManager = NULL;
    EnemyManager * enemyManager = NULL;
    bool upForAir = true;
    int level = 0;
    int oxygenLevel = 200;

    Seaquest() : Game() {
      player = new Player();
      bullet = new Bullet();
      oxygenBar = new OxygenBar();
      air = new Air();
      reset();
    }

    void reset() {
      diverManager = new DiverManager();
      enemyManager = new EnemyManager();
      oxygenLevel = 200;
      upForAir = true;
    }

    void nextLevel() {
      level++;
      pointsFromEnemyShot = std::min(20+10*level, 90);
      upForAir = true;
      enemyManager->nextLevel();
      resetOxygenBar() ;   
      bullet->setIsAlive(false);
      player->setX(32);
      player->setY(3);
      player->direction = RIGHT;
    }

    void resetOxygenBar() {
      oxygenBar->setWidth(50);
      oxygenLevel = 200;
    }

    void updateOxygenBar() {

      oxygenLevel--;
      if(oxygenLevel % 20 == 0) {
        oxygenBar->setWidth(oxygenBar->getWidth()-5);
      }

    }

    void fireBullet() {
      bullet->setY(player->getY()+player->getHeight()/2);
      bullet->setIsAlive(true);
      if(player->direction == LEFT) {
        bullet->setX(player->getX());
        bullet->setVx(-2);
      } else {
        bullet->setX(player->getX()+player->getWidth());
        bullet->setVx(2);
      }
    }

    int rescueDivers(int diverCount) {
      int score = 0;
      for(int i=0; i<diverCount; i++) {
        score += pointsFromDiverSaved;
        pointsFromDiverSaved = std::min(1000, pointsFromDiverSaved+50);
      }
      return score;
    }

    void renderPixels(uint8_t * buffer) override {
      memset(buffer, 0, sizeof(uint8_t) * 64 * 64);
      air->render(buffer);
      player->render(buffer);
      bullet->render(buffer);
      oxygenBar->render(buffer);
      
      for(Diver * diver : diverManager->divers){
        diver->render(buffer);
      }
      for(Diver * diver : diverManager->diversSaved){
        diver->render(buffer);
      }
      for(Submarine * submarine : enemyManager->submarines){
        submarine->render(buffer);
      }
      for(Shark * shark : enemyManager->sharks){
        shark->render(buffer);
      }
      for(EnemyBullet * enemyBullet : enemyManager->bullets){
        enemyBullet->render(buffer);
      }
    }

    std::vector<int> step(uint8_t action) override {
      bool NoAction = (action == 0);
      bool Fire = (action == 1);
      bool Up = (action == 2);
      bool Down = (action == 3);
      bool Left = (action == 4);
      bool Right = (action == 5); 
      
      int score = 0;
      
      bool playerMovement = true;

      player->setVx(0);
      player->setVy(0);

      if(Up && player->getY() > MIN_PLAYER_Y) {
        player->setVy(-1);
      } else if(Down && player->getY() < MAX_PLAYER_Y) {
        player->setVy(1);
      } else if(Left && player->getX() > MIN_PLAYER_X) {
        player->direction = LEFT;
        player->setVx(-1);
      } else if(Right && player->getX() < MAX_PLAYER_X) {
        player->direction = RIGHT;
        player->setVx(1);       
      } else if(Fire && !bullet->getIsAlive()) {
        fireBullet();
        playerMovement = false;
      } else if(NoAction) {
        playerMovement = false;
      }

      if(playerMovement) {
        player->update();
      }

      if(bullet->getIsAlive()) {
        if(bullet->getX() <= 0 || bullet->getX() >= 64) {
          bullet->setIsAlive(false);
        } else {
          bullet->update();
        }
      }

      GAME_EVENT enemyEvent = enemyManager->update(player, bullet);

      if(enemyEvent == LOSE) {
        return {score, true};
      } else if(enemyEvent == ENEMY_SHOT) {
        score += pointsFromEnemyShot;
      }

      diverManager->update(player);

      if(upForAir && player->getY() > 3) {
        upForAir = false;
      }
      else if(!upForAir && player->getY() <= 3) {
        GAME_EVENT diverEvent = diverManager->retrieveOxygen();

        if(diverEvent == RESCUE) {
          upForAir = true;
          score += rescueDivers(1);
          score += oxygenLevel;
          resetOxygenBar();
        } else if(diverEvent == NEXT_LEVEL) {
          score += rescueDivers(6);
          score += oxygenLevel;
          nextLevel();
        } else if(diverEvent == LOSE) {
          return {score, true};
        }
            
      }

      if(!upForAir) {
        updateOxygenBar();
      }

      if(oxygenLevel <= 0) {
        return {score, true};
      }

      return {score, false};
    }

    std::vector<bool> legalActions() override {
      //      NOOP, FIRE, UP,   DOWN,   LEFT, RIGHT
      return {true, true, true, true,  true, true};
    }

};