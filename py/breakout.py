from .game import Game
from .ball import Ball
import pygame
from .colors import LIGHTBLUE, RED, ORANGE, YELLOW
from random import randint

class Breakout(Game):

    def __init__(self):
        super().__init__()
        self.paddle_speed = 120
        self.paddle = None
        self.ball = None
        self.reset()


    def reset(self):
        super().reset()
      
        self.paddle = self.createNewEntity(LIGHTBLUE, 100, 10, 200, 600)

        self.ball   = self.createNewEntity(LIGHTBLUE, 20, 20, 200, 300, game_entity_type=Ball)
        
        self.bricks = pygame.sprite.Group()
        
        brick_colors = [RED, RED, ORANGE, ORANGE, YELLOW, YELLOW]
        for i in range(len(brick_colors)):
            for j in range(9):
                brick = self.createNewEntity(brick_colors[i], 60, 30, 30 + j*65, 60+i*40)
                self.bricks.add(brick)

    def update(self, action):

        left  = (action == 4)
        right = (action == 5)
        score = 0
        paddleX = self.paddle.x

        if left:
            paddleX  = paddleX - self.paddle_speed*self.dt 
        elif right:
            paddleX  = paddleX + self.paddle_speed*self.dt 

        self.paddle.x = max(0, min(self.screen_width - self.paddle.width  , paddleX))

        if self.ball.x >= self.screen_width - self.ball.width or self.ball.x <= 0:
            self.ball.vx = -1*(self.ball.vx+5*randint(-1,1))

        if self.ball.y < self.ball.height:
            self.ball.vy = abs(self.ball.vy)
            self.ball.vx = -1*self.ball.vx

        if self.ball.y >= self.paddle.y + self.ball.height:
            return (score, True)

        if pygame.sprite.collide_mask(self.ball, self.paddle):
            self.ball.vx = self.ball.vx * (-1 if randint(0,1)==1 else 1)
            self.ball.vy = -1*(self.ball.vy+10*randint(-1,1))

        self.ball.x = max(0, min(
            self.screen_width - self.ball.width,
            self.ball.x + self.ball.vx * self.dt
        ))

        self.ball.y += self.ball.vy * self.dt

        brickCollisionList = pygame.sprite.spritecollide(self.ball, self.bricks, False)

        for brick in brickCollisionList:
            self.ball.vx = -self.ball.vx+randint(-10, 10)
            self.ball.vy =  max(abs(self.ball.vy)+(30*randint(-1,1)),60)
            score += 1
            brick.kill()

            if len(self.bricks)==0:
                return (score, True)

        return (score, False)
            
    def legalActions(self):
        return [True, False, False, False, True, True, False, False, False, False]