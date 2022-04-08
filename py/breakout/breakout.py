from py.game import Game
#from ..game_entity import GameEntity
from py.cgame_entity import CGameEntity
from py.breakout.ball import Ball
from py.colors import *
from random import randint

class Breakout(Game):

    def __init__(self):
        super().__init__()
        self.paddle_speed = 1
        self.paddle = None
        self.ball = None
        self.reset()


    def reset(self):
        
        self.paddle = CGameEntity(10, 2, 20, 60, WHITE)
        self.ball   = Ball(2, 2, 20, 30, WHITE)
        self.bricks = []
        for i in range(len(BORING_COLORS)):
            for j in range(9):
                brick = CGameEntity(8, 3, 4 + j*6, 4+i*3, BORING_COLORS[i]) 
                self.bricks.append(brick)
           
        
        self.gameEntities.extend(self.bricks)
        self.gameEntities.extend([self.ball,self.paddle])


    def step(self, action):

        left  = (action == 4)
        right = (action == 5)
        score = 0
        paddleX = self.paddle.x

        if left:
            paddleX  = paddleX - self.paddle_speed
        elif right:
            paddleX  = paddleX + self.paddle_speed 

        self.paddle.x = max(0, min(self.screen_width - self.paddle.width - 1, paddleX))

        if self.ball.x >= self.screen_width - self.ball.width or self.ball.x <= 0:
            self.ball.vx = -(self.ball.vx+0.1*randint(-1,1))

        if self.ball.y < self.ball.height:
            self.ball.vy = -self.ball.vy
            self.ball.vx = -self.ball.vx

        if self.ball.y >= self.paddle.y + self.ball.height:
            return (score, True)

        if self.ball.collide(self.paddle):
            self.ball.vx = self.ball.vx * (-1 if randint(0,1)==1 else 1)
            self.ball.vy =  -self.ball.vy
         
        self.ball.x = max(0, min(
            self.screen_width - self.ball.width,
            self.ball.x + self.ball.vx
        ))

        self.ball.y += self.ball.vy

        for brick in self.bricks:

            if self.ball.collide(brick):
                
                self.gameEntities.remove(brick)
                self.ball.vx = -self.ball.vx+0.1*randint(-1, 2)
                self.ball.vy =  -self.ball.vy+0.1*randint(-1, 3)
                score += 1
                self.bricks.remove(brick)
        
                if len(self.bricks)==0:
                    return (score, True)

                break

        return (score, False)
            
    def legalActions(self):
        return (True, False, False, False, True, True)