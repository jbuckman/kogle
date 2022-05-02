from py.game import Game
from py.cgame_entity import CGameEntity
from py.colors import *
from random import random, randint, uniform
from py.asteroids.rocks import RockManager
import numpy as np
from math import sqrt

MAX_BULLETS = 10
MAX_BULLET_FRAME_THRESHOLD = 5
TOTAL_ROTATIONS = 12
FRAMES_TILL_NEXT_LEVEL = 300

class Asteroids(Game):

    def __init__(self):
        super().__init__()

        sqrt3half = sqrt(3)*0.5

        self.clockwise = np.array([
            [sqrt3half, -0.5], 
            [0.5, sqrt3half]]
        )
        self.counter_clockwise = np.array([
            [sqrt3half, 0.5], 
            [-0.5, sqrt3half]]
        )

        self.reset()
    
    def reset(self):

        self.player = CGameEntity(3, 3, 32, 32, WHITE)
        self.thruster = 0

        self.ufo = CGameEntity(3, 2, 32, 32, BORING_COLORS[3], False)
        self.ufo_bullet = CGameEntity(1, 1, 32, 32, BORING_COLORS[2], False)
        self.ufo_spawn_rate = [60, 100]
        self.next_ufo_spawn = randint(*self.ufo_spawn_rate)
        self.ufo_bullet_probability = 0.8

        self.level_counter = 0
        self.level = 1

        self.rotation_index = 0

        self.dot = CGameEntity(1, 1, 33, 32, BORING_COLORS[1], True)
        self.horizontalBar = CGameEntity(3, 1, 32, 33, BORING_COLORS[1], True)
        self.verticalBar = CGameEntity(1, 3, 33, 32, BORING_COLORS[1], False)

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

        self.gameEntities.add(self.dot)
        self.gameEntities.add(self.horizontalBar)
        self.gameEntities.add(self.verticalBar)

        self.gameEntities.extend(self.bullets)
        self.gameEntities.add(self.ufo)
        self.gameEntities.add(self.ufo_bullet)

    def step(self, action):

        score = 1
        fire  = (action == 1)
        forward  = (action == 2)
        back = (action == 3)
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
            self.rotation_index = (self.rotation_index - 1) % TOTAL_ROTATIONS

        elif right:

            self.direction = np.dot(self.clockwise ,self.direction)
            self.rotation_index = (self.rotation_index + 1) % TOTAL_ROTATIONS
 
        if forward:
            self.thruster =  min(self.thruster+1, 1)
        elif back:
            self.thruster =  max(self.thruster-1, -1)

        if self.thruster != 0:
            self.player.vx = self.direction[0]*self.thruster*0.5
            self.player.vy = self.direction[1]*self.thruster*0.5

            if self.player.y <= 0:
                self.player.y = self.screen_height - self.player.height
                self.player.x = self.screen_width - self.player.x 
            elif self.player.y >= self.screen_height - self.player.height:
                self.player.y = 1
                self.player.x = self.screen_width - self.player.x 
            elif self.player.x <= 0:
                self.player.y = self.screen_height - self.player.y
                self.player.x = self.screen_width - self.player.width
            elif self.player.x >= self.screen_width - self.player.width:
                self.player.y = self.screen_height - self.player.y
                self.player.x = 1
 

        else:
            self.player.vx = 0
            self.player.vy = 0
      

        self.player.update()
        self.updateIndicator()   
        self.next_ufo_spawn -= 1

        if self.next_ufo_spawn <= 0 and not self.ufo.isAlive:
            self.spawnUFO()

        if self.ufo.isAlive:
            if self.player.collide(self.ufo):
                return (score, True)
            elif self.ufo.y <=  self.ufo.height or\
            self.ufo.x <=  self.ufo.height or\
            self.ufo.y >=  self.screen_height - self.ufo.height or\
            self.ufo.x >=  self.screen_width - self.ufo.width:
                self.ufo.isAlive = False
            else:

                if not self.ufo_bullet.isAlive and random() > self.ufo_bullet_probability:
                    self.ufo_bullet.isAlive = True
                    self.ufo_bullet.x = self.ufo.x+2
                    self.ufo_bullet.y = self.ufo.y+1
                    self.ufo_bullet.vx = uniform(-1, 1)
                    self.ufo_bullet.vy = uniform(-1, 1)
                elif self.ufo_bullet.isAlive:

                    if self.ufo_bullet.y <=  self.ufo_bullet.height or\
                    self.ufo_bullet.x <=  self.ufo_bullet.height or\
                    self.ufo_bullet.y >=  self.screen_height - 1 or\
                    self.ufo_bullet.x >=  self.screen_width - 1:
                        self.ufo_bullet.isAlive = False
                    elif self.ufo_bullet.collide(self.player):
                        return (score, True)
                    else:
                        self.ufo_bullet.update()
                    
                self.ufo.update()  

        for bullet in self.bullets:
            if bullet.isAlive:
                if bullet.y <=  bullet.height or\
                bullet.x <=  bullet.height or\
                bullet.y >=  self.screen_height - 1 or\
                bullet.x >=  self.screen_width - 1:
                    bullet.isAlive = False
                elif self.ufo.isAlive and self.ufo.collide(bullet):
                    self.ufo.isAlive = False
                    self.ufo_bullet.isAlive = False
                    bullet.isAlive = False
                    score += 100
                    self.level_counter += 100
                    self.next_ufo_spawn = randint(*self.ufo_spawn_rate)
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
                self.level_counter += score
                self.rockManager.kill(rock)

        self.level_counter += 1

        if self.level_counter >= FRAMES_TILL_NEXT_LEVEL * self.level:
            
            self.level_counter = 0 
            self.level += 1
            self.increaseDifficulty()


        return (score, False)

    def legalActions(self):
        return (True, True, True, True, True, True)

    def spawnUFO(self):
        random_num = random()
        self.ufo.isAlive = True
        if random_num <= 0.25:
            self.ufo.x = randint(self.ufo.width+1, 32-self.ufo.width) 
            self.ufo.y = self.ufo.height+1
            self.ufo.vx = uniform(0.1, 0.5)
            self.ufo.vy = uniform(0.1, 0.5)

        elif random_num <= 0.5:
            self.ufo.x = randint(32, 64-self.ufo.width) 
            self.ufo.y = self.ufo.height+1
            self.ufo.vx = -uniform(0.1, 0.5)
            self.ufo.vy = uniform(0.1, 0.5)

        elif random_num <= 0.75:   
            self.ufo.x = randint(self.ufo.width+1, 32-self.ufo.width) 
            self.ufo.y = 64-self.ufo.height-1
            self.ufo.vx = -uniform(0.1, 0.5)
            self.ufo.vy = uniform(0.1, 0.5)

        else:
            self.ufo.x = randint(32, 64-self.ufo.width) 
            self.ufo.y = 64-self.ufo.height-1
            self.ufo.vx = -uniform(0.1, 0.5)
            self.ufo.vy = -uniform(0.1, 0.5)

    def increaseDifficulty(self):

        if self.rockManager.spawnRate >= 25:
            self.rockManager.spawnRate = self.rockManager.spawnRate - 3

        if self.ufo_spawn_rate[0] >= 35:
            self.ufo_spawn_rate[0] -= 5
            self.ufo_spawn_rate[1] -= 5

        if self.ufo_bullet_probability > 0.2:
            self.ufo_bullet_probability -= 0.05


    def updateIndicator(self):

        self.horizontalBar.x = self.player.x 
        self.horizontalBar.y = self.player.y + 1

        self.verticalBar.x = self.player.x + 1 
        self.verticalBar.y = self.player.y

        if self.rotation_index == 0:
            self.horizontalBar.isAlive = True
            self.verticalBar.isAlive = False
            self.dot.x = 1 + self.player.x
            self.dot.y = self.player.y
        elif self.rotation_index == 1:
            self.horizontalBar.isAlive = True
            self.verticalBar.isAlive = False
            self.dot.x = 2 + self.player.x
            self.dot.y = self.player.y
        elif self.rotation_index == 2:
            self.horizontalBar.isAlive = False
            self.verticalBar.isAlive = True
            self.dot.x = 2 + self.player.x
            self.dot.y = self.player.y
        elif self.rotation_index == 3:
            self.horizontalBar.isAlive = False
            self.verticalBar.isAlive = True
            self.dot.x = 2 + self.player.x
            self.dot.y = 1 + self.player.y
        elif self.rotation_index == 4:
            self.horizontalBar.isAlive = False
            self.verticalBar.isAlive = True
            self.dot.x = 2 + self.player.x
            self.dot.y = 2 + self.player.y
        elif self.rotation_index == 5:
            self.horizontalBar.isAlive = True
            self.verticalBar.isAlive = False
            self.dot.x = 2 + self.player.x
            self.dot.y = 2 + self.player.y
        elif self.rotation_index == 6:
            self.horizontalBar.isAlive = True
            self.verticalBar.isAlive = False
            self.dot.x = 1 + self.player.x
            self.dot.y = 2 + self.player.y
        elif self.rotation_index == 7:
            self.horizontalBar.isAlive = True
            self.verticalBar.isAlive = False
            self.dot.x = self.player.x
            self.dot.y = 2 + self.player.y
        elif self.rotation_index == 8:
            self.horizontalBar.isAlive = False
            self.verticalBar.isAlive = True
            self.dot.x = self.player.x
            self.dot.y = 2 + self.player.y
        elif self.rotation_index == 9:
            self.horizontalBar.isAlive = False
            self.verticalBar.isAlive = True
            self.dot.x = self.player.x
            self.dot.y = 1 + self.player.y
        elif self.rotation_index == 10:
            self.horizontalBar.isAlive = False
            self.verticalBar.isAlive = True
            self.dot.x = self.player.x
            self.dot.y = self.player.y
        elif self.rotation_index == 11:
            self.horizontalBar.isAlive = True
            self.verticalBar.isAlive = False
            self.dot.x = self.player.x
            self.dot.y = self.player.y