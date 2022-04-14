from py.game import Game
from py.cgame_entity import CGameEntity
from py.colors import *
from random import random, randint
from py.asteroids.rocks import RockManager
import numpy as np


MAX_BULLETS = 10
MAX_BULLET_FRAME_THRESHOLD = 5


class Asteroids(Game):

 
    def __init__(self):
        super().__init__()

        self.clockwise = np.array([
            [0.9848, -0.173648], 
            [0.173648, 0.9848]]
        )

        self.counter_clockwise = np.array([
            [0.9848, 0.173648], 
            [-0.173648, 0.9848]]
        )

        self.reset()
    

    def reset(self):

        self.player = CGameEntity(3, 3, 32, 32, WHITE)
        self.rockManager = RockManager()

        self.bullets = [
            CGameEntity(1, 1, 33, 33, BORING_COLORS[4], False)
            for _ in range(MAX_BULLETS)
        ]
        self.bullet_index = 0
        self.bullet_delay = 0

        self.direction = np.array([0, -1])
        self.gameEntities.extend(self.rockManager.rocks)
        self.gameEntities.add(self.player)
        self.gameEntities.extend(self.bullets)


    def step(self, action):
        score = 1

        fire  = (action == 1)
        forward  = (action == 2)
        left  = (action == 4)
        right = (action == 5)

        if fire and self.bullet_index <= MAX_BULLETS and self.bullet_delay == 0:
            
            self.bullets[self.bullet_index].isAlive = True
            self.bullets[self.bullet_index].x = self.player.x+1
            self.bullets[self.bullet_index].y = self.player.y+1
            self.bullets[self.bullet_index].vx = self.direction[0]
            self.bullets[self.bullet_index].vy = self.direction[1]
            self.bullet_index = (self.bullet_index+1) % MAX_BULLETS 
            self.bullet_delay = MAX_BULLET_FRAME_THRESHOLD 
            

        if left: 
            self.direction = np.dot(self.counter_clockwise ,self.direction)
        elif right:
            self.direction = np.dot(self.clockwise ,self.direction)
        
        if forward:
            self.player.vx = self.direction[0]
            self.player.vy = self.direction[1]
            self.player.update()
        else:
            self.player.vx = 0
            self.player.vy = 0


        for bullet in self.bullets:
            if bullet.isAlive:
                if bullet.y <=  bullet.height or\
                bullet.x <=  bullet.height or\
                bullet.y >=  self.screen_height - 1 or\
                bullet.x >=  self.screen_width - 1:
                    bullet.isAlive = False
                else:
                    for rock in self.rockManager.activeRocks:
                        if bullet.collide(rock):
                            self.rockManager.kill(rock)
                            bullet.isAlive = False
                            break
                    else:
                        bullet.update()
        
        if self.bullet_delay > 0:
            self.bullet_delay -= 1

        self.rockManager.update()

        for rock in self.rockManager.activeRocks:
            if rock.collide(self.player):
                return (score, True)        
            elif rock.x >= 64 - rock.width or\
            rock.x <= rock.width or\
            rock.y >= 64 - rock.height or\
            rock.y <= rock.height:

                score += 10 * (4-rock.size)
                self.rockManager.kill(rock)
                

        return (score, False)


    def legalActions(self):
        return (True, True, True, False, True, True)


        
