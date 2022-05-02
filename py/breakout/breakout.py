
from py.game import Game
from py.cgame_entity import CGameEntity
from py.colors import *
from math import floor
from random import random

class Brick(CGameEntity):

    def __init__(self, _col, _row, _color):
        super().__init__(8, 3, _col*8, 8+_row*3, _color)
        self._col = _col
        self._row  = _row
    
    @property
    def row(self):
        return self._row
    
    @property
    def col(self):
        return self._col   

class Breakout(Game):

    def __init__(self):
        super().__init__()
        self.slow_speed = 0.35
        self.reset()

    def reset(self):
        
        self.paddle = CGameEntity(10, 2, 20, 60, WHITE)
        self.ball   = CGameEntity(2, 2, 20, 30, WHITE)
        self.ball.vx = -self.slow_speed / 2.0 if random() < 0.5 == 0 else self.slow_speed / 2.0
        self.ball.vy = self.slow_speed
        self.speed = self.slow_speed
        self.volley = 0
        self.bricks = []
        
        for i in range(len(BORING_COLORS)):
            self.bricks.extend([Brick(j, i, BORING_COLORS[i]) for j in range(8)])
        
        self.brick_count = len(self.bricks)           
        self.gameEntities.extend(self.bricks)
        self.gameEntities.extend([self.ball,self.paddle])

    def doVolley(self):
        self.volley += 1
        if self.volley % 4 == 0 and self.speed < self.slow_speed * 2.5:
            self.speed += self.slow_speed / 3.0

        section = floor(self.ball.x - self.paddle.x - 1)
        section = 0 if section <= 0 else section
        section = 7 if section >= 7 else section 

        if section == 0:
            self.ball.vx = -self.speed*2
        elif section == 1:
            self.ball.vx = -self.speed*1.5
        elif section == 2:
            self.ball.vx = -self.speed
        elif section == 3:
            self.ball.vx = -self.speed*0.5
        elif section == 4:
            self.ball.vx = self.speed*0.5
        elif section == 5:
            self.ball.vx = self.speed
        elif section == 6:
            self.ball.vx = 1.5*self.speed
        elif section == 7:
            self.ball.vx = 2*self.speed


        self.ball.vy =  -self.ball.vy

    def step(self, action):

        left  = (action == 4)
        right = (action == 5)
        score = 0
  
        if left and self.paddle.x >= 1:
            self.paddle.vx = -1
            self.paddle.update()
        elif right and self.paddle.x <= 54:
            self.paddle.vx = 1
            self.paddle.update()
        else:
            self.paddle.vx = 0
        if self.ball.y >= self.paddle.y + self.ball.height:
            return (score, True)        
        elif self.ball.x >= self.screen_width - self.ball.width or self.ball.x <= 0:
            self.ball.vx = -self.ball.vx
        elif self.ball.collide(self.paddle):
            self.doVolley()
        elif self.ball.y < self.ball.height:
            self.ball.vy = -self.ball.vy
        else:  
            for brick in self.bricks:

                if brick.isAlive and self.ball.collide(brick):
               
                    self.ball.vy = -self.ball.vy
                    score += (6-brick.row)
                    brick.isAlive = False
                    self.brick_count -= 1
            
                    if self.brick_count==0:
                        return (score, True)

                    break
                
        self.ball.update()
        return (score, False)
            
    def legalActions(self):
        return (True, False, False, False, True, True)