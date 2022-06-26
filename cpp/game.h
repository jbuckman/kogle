#pragma once
#include <vector>
#include <time.h>
#include <random>

class Game {
  

  public: 
  
    static bool bernoulli(double probability) {
      std::random_device randomDevice{}; 
      std::mt19937 generator{randomDevice()}; 
      std::bernoulli_distribution distribution(probability);
      return distribution(generator);
    }

    static double uniform(double min, double max) {
      std::random_device randomDevice{}; 
      std::mt19937 generator{randomDevice()}; 
      std::uniform_real_distribution <>distribution(min, max);
      return distribution(generator);
    }

    static double uniformInteger(int min, int max) {
      std::random_device randomDevice{}; 
      std::mt19937 generator{randomDevice()}; 
      std::uniform_int_distribution<> distribution(min, max);
      return distribution(generator);
    }

    static int pmf(std::vector<int> p) {
      std::random_device randomDevice{}; 
      std::mt19937 generator{randomDevice()}; 
      std::discrete_distribution<int> distribution(p.begin(), p.end());
      return distribution(generator);
    }

    static int randomBetween(int min, int max) {
      return (rand() % max + min);
    }

    static int diceRoll() {
      return randomBetween(1,6);
    }

    enum Action {NO_ACTION=0, FIRE=1, UP=2, DOWN=3, LEFT=4, RIGHT=5};
    
    void drawRectDangerously(uint8_t * buffer, float x, float y, float w, float h, uint8_t v) {
      for (float j = y; j < y + h; j++) {
        for (float i = x; i < x + w; i++) {
          int index = floor(j) * 64 + floor(i);
          buffer[index] = v;
        }
      }   
    };


    virtual std::vector<bool> legalActions() = 0;
    virtual void renderPixels(uint8_t * buffer) = 0;
    virtual std::vector<int> step(uint8_t action) = 0;
    
    Game() {
      srand(time(NULL));
    }

    static void drawPixel(uint8_t * buffer, float x, float y, uint8_t v) {
      if (x < 0 || x >= 64 || y < 0 || y >= 64) return;
      int i = floor(y) * 64 + floor(x);
      buffer[i] = v;
    };


    class GameEntity {
      protected: 
        int * sprite;
        bool isSprite;
      private:
        float x;
        float y;
        float vx;
        float vy;
        float width;
        float height;
        uint8_t color;
        bool isAlive; 
      public: 
        GameEntity(float x, float y, float width, float height, uint8_t color, bool isAlive) {
          this->x = x;
          this->y = y;
          this->vx = 0;
          this->vy = 0;
          this->width = width;
          this->height = height;
          this->color = color;
          this->isAlive = isAlive;
          this->isSprite = false;
        }

        float getX() {return x;}
        void setX(float x) {
          this->x = x;
        }

        float getY() {return y;}
        void setY(float y) {
          this->y = y;
        }

        float getVx() {return vx;}
        void setVx(float vx) {
          this->vx = vx;
        }

        float getVy() {return vy;}
        void setVy(float vy) {
          this->vy = vy;
        }

        float getWidth() {return width;}
        void  setWidth(float width) {
          this->width = width;
        } 

        int getHeight() {return height;}
        void setHeight(float height) {
          this->height = height;
        }

        uint8_t getColor() {return color;}
        void setColor(uint8_t color) {
          this->color = color;
        }

        bool getIsAlive() {return isAlive;}
        void setIsAlive(bool isAlive) {
          this->isAlive = isAlive;
        }  

        bool getIsSprite() {return isSprite;}
        void setIsSprite(bool isSprite) {
          this->isSprite = isSprite;
        }  

        bool collide(GameEntity& other) {
          
          if(!other.getIsAlive()) 
            return false;
          return collide(other.getX(), other.getY(), other.getWidth(), other.getHeight());
        };

        bool collide(float x, float y, float width, float height) {
          if(!isAlive) 
            return false;

          return (abs((this->getX() + this->getWidth()/2.0) -  (x + width/2.0)) * 2 
                 < (this->getWidth() + width)) &&
                 
                 (abs((this->getY() + this->getHeight()/2.0) - (y + height/2.0)) * 2 
                 < (this->getHeight() + height));
        };

        bool collide(GameEntity * other) {
          if(!other->getIsAlive()) 
            return false;          
          
          return collide(other->getX(), other->getY(), other->getWidth(), other->getHeight());
        };

        void update() {
          if(isAlive) {
            x += vx;
            y += vy;
          }
        }

      void render(uint8_t * buffer) {
 
        if(isAlive) {

          for (int i = 0; i < (int)height; i++) {
            for (int j = 0; j < (int)width; j++) {    

              if(!isSprite || (*(sprite + (i * (int)width + j)) == 1)) {  
                drawPixel(buffer, j+x, i+y, color);
              }
            }
          }        
        }
      }

    };
  

};
