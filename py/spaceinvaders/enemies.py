from py.cgame_entity import CGameEntity
from py.colors import *
from random import random, choice
from py.spaceinvaders.direction import Direction
from py.spaceinvaders.game_event import GameEvent
import numpy as np

class Bullet(CGameEntity):
    def __init__(self):
        super().__init__(1, 2, 0, 0, BORING_COLORS[0], False)

class Enemy(CGameEntity):
    def __init__(self, _row, _col):
        super().__init__(3, 3, 6+4*_row+_row*3, _col*5+6, BORING_COLORS[0])
        self._row = _row
        self._col = _col
        self.reset()

    @property
    def row(self):
        return self._row

    @property
    def col(self):
        return self._col

    def reset(self):
        self.x = 6+4*self._row+self._row*3
        self.y = self._col*5+6
        self.isAlive = True
        

ROW_LENGTH = 6
COLUMN_LENGTH = 4
TOTAL = COLUMN_LENGTH*ROW_LENGTH
MAX_BULLETS = 4

class EnemyManager:
    def __init__(self):
        self._enemies = []
        self._bullets = [Bullet() for _ in range(MAX_BULLETS)] 
        self.difficulty = 0
        self.chance_of_fire = 0.8
        self.reset()
        for r in range(ROW_LENGTH):
            self._enemies.extend([Enemy(r, c) for c in range(COLUMN_LENGTH)])    
    
    def reset(self):
        self.direction = Direction.RIGHT
        self.enemyMap = np.array([[1]*COLUMN_LENGTH]*ROW_LENGTH)
        self.enemyCount = COLUMN_LENGTH*ROW_LENGTH
        bullets_amount =  min(MAX_BULLETS, self.difficulty+1)
        self.chance_of_fire = max(0.4, 0.8 - self.difficulty*0.1) 
        for bullet in self._bullets:
            bullet.isAlive = False  
        self._free_bullets = self._bullets[0:bullets_amount]
        self._active_bullets = []

    def nextLevel(self):
        self.difficulty += 1
        for enemy in self._enemies:
            enemy.reset()
        self.reset()

    @property
    def enemies(self):
        return self._enemies

    @property
    def bullets(self):
        return self._bullets

    @property
    def active_enemey_bullets(self):
        return self._active_bullets

    def despawnBullet(self, bullet):
        bullet.isAlive = False
        self._free_bullets.append(bullet)
        self._active_bullets.remove(bullet)       

    def update(self, player_bullet):

        if self.direction == Direction.RIGHT:
            rows = range(ROW_LENGTH-1, -1,-1)
            cols = range(COLUMN_LENGTH-1, -1,-1)
        else:
            rows = range(ROW_LENGTH)
            cols = range(COLUMN_LENGTH)
        
        descend = False
        event = GameEvent.NOTHING
        for row in rows:
            for col in cols:
                enemy = self._enemies[col+(row*COLUMN_LENGTH)]
                vx = 0
                vy = 0
                if enemy.isAlive:

                    if player_bullet.isAlive and enemy.collide(player_bullet):
                       enemy.isAlive = False
                       self.enemyMap[enemy.row][enemy.col] = 0
                       player_bullet.isAlive = False
                       self.enemyCount -= 1
                       event = GameEvent.ENEMY_SHOT
                       continue

                    if not descend:
            
                        if enemy.x >= 61 or enemy.x <= 0:
                            descend = True
                            self.direction = self.direction ^ 1 
                        else:
                            vx = -1 if self.direction == Direction.LEFT else 1

                    if descend:
                        vx = -1 if self.direction == Direction.LEFT else 1
                        vy = 1
                    
                    enemy.vx = vx
                    enemy.vy = vy
                
                    enemy.update()
                    
                    if enemy.y >= 50:
                        return GameEvent.GAME_OVER

        if len(self._free_bullets) > 0 and self.chance_of_fire > random():
            
            row, col = choice(np.argwhere(self.enemyMap==1))
            enemy = self._enemies[col+(row*COLUMN_LENGTH)]
            bullet = self._free_bullets.pop()
            bullet.isAlive = True
            bullet.x = enemy.x+int(enemy.width/2)
            bullet.y = enemy.y+enemy.height
            bullet.vy = 1
            bullet.update()
            self._active_bullets.append(bullet)

        if self.enemyCount == 0:
            event = GameEvent.NEXT_LEVEL
        
        return event
