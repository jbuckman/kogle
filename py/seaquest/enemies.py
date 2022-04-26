from kogle.kogle import LEFT
from py.cgame_entity import CGameEntity
from py.colors import *
from random import random, randint
from collections import deque
from py.seaquest.game_event import GameEvent
from py.seaquest.direction import Direction

class Enemy(CGameEntity):
    def __init__(self, _width, _height, _color):
        super().__init__(_width, _height, 33, 33, _color, False)
 
    @property 
    def direction(self):
        return self._direction

    @property
    def canFire(self):
        return self._canFire

    @property
    def enemyType(self):
        return ''

    def update(self):
        if self.isAlive:
            self.vx =  -self._speed if self._direction == Direction.LEFT else self._speed
            super().update()

    def spawn(self, _x, _y, _speed, _direction):
        self.isAlive = True
        self.x = _x
        self.y = _y
        self._speed = _speed
        self._direction = _direction
        
class EnemyBullet(Enemy):

    def __init__(self):
        super().__init__(2, 1, BORING_COLORS[2])

    @property
    def enemyType(self):
        return 'bullet'

    def spawn(self, _x, _y, _speed, _direction):
        self.isAlive = True
        self.y = _y
        self.x =  _x
        self._direction = _direction
        self._speed = _speed
        self.vx =  -_speed if _direction == Direction.LEFT else _speed

class Shark(Enemy):
    def __init__(self):
        super().__init__(3, 3, BORING_COLORS[2])
        self.flicker = True

    @property
    def enemyType(self):
        return 'shark'

    def update(self):
        
        self.vy = 1 if self.flicker else -1
        self.flicker ^= 1
        super().update()
    

class Submarine(Enemy):
    def __init__(self):
        super().__init__(5, 3, BORING_COLORS[1])
        self._hasFired = False
    
    @property
    def hasFired(self):
        return self._hasFired
    
    @hasFired.setter
    def hasFired(self, value):
        self._hasFired = value

    @property
    def enemyType(self):
        return 'submarine'

    def update(self):
        super().update()


MAX_SHARKS = 10
MAX_SUBS = 6
MAX_BULLETS = 6

class EnemyManager:
    def __init__(self):
        self._frameNextSharkBounds = [20, 40]
        self._frameNextSubBounds = [100, 150]
        self._frameTillNextSharkSpawn = 5 #randint(*self._frameNextEnemyBounds)
        self._frameTillNextSubSpawn = 20 
        self._enemies = []
        self._enemies_spawned = 0
        self._active_enemies = []
        self._difficulty = 0 

        sharks_list = [Shark() for _ in range(MAX_SHARKS)]
        subs_list = [Submarine() for _ in range(MAX_SUBS)]
        bullet_list = [EnemyBullet() for _ in range(MAX_BULLETS)]

        self.enemy_dict = {
            'shark': deque(sharks_list), 
            'submarine': deque(subs_list),
            'bullet': deque(bullet_list),
        }
  
        self._enemies.extend(sharks_list)  
        self._enemies.extend(subs_list)
        self._enemies.extend(bullet_list)   



    @property
    def enemies(self):
        return self._enemies

    def _spawn(self, enemies):
        
        amount = min(len(enemies), randint(1,2))
        pos = []
        
        if amount == 1:
            pos = [randint(7, 56)]
        elif amount == 2:
            pos = [randint(7, 30), randint(34, 56)]

        for i in range(amount):
            direction = randint(0,1)
            x = 70 if direction == Direction.LEFT else -10
            enemySpeed = 1
            if self._difficulty == 1:
                enemySpeed +=  1 if random() > 0.8 else 0
            elif self._difficulty == 2:
                enemySpeed +=  1 if random() > 0.7 else 0
            elif self._difficulty >= 3:
                enemySpeed +=  1 if random() > 0.5 else 0
            enemy = enemies.popleft()
            enemy.spawn(x, pos[i], enemySpeed, direction)
            self._active_enemies.append(enemy)
            self._enemies_spawned += 1

    def nextLevel(self):
        self._difficulty += 1
        if self._difficulty >= 2:
            self._frameNextSharkBounds[0] = max(10,self._frameNextSharkBounds[0]-2) 
            self._frameNextSharkBounds[1] = max(30,self._frameNextSharkBounds[1]-2) 
            self._frameNextSubBounds[0] = max(50,self._frameNextSubBounds[0]-10) 
            self._frameNextSubBounds[1] = max(100,self._frameNextSubBounds[1]-10) 
        for enemy in self._active_enemies:
           enemy.isAlive = False
           self.enemy_dict[enemy.enemyType].append(enemy)
        self._active_enemies = []
        self._enemies_spawned = 0

    def fireBullet(self, enemy):

        bullets = self.enemy_dict['bullet']
        if len(bullets) > 0:
            bullet = bullets.popleft()
            y = enemy.y+(enemy.height)/2
            x = enemy.x if enemy.direction == Direction.LEFT else enemy.x+enemy.width
            self._active_enemies.append(bullet)
            bullet.spawn(x, y, 2, enemy.direction)
            self._enemies_spawned += 1

                
    def update(self, player, bullet):
        
        event = GameEvent.NOTHING
        self._frameTillNextSharkSpawn -= 1
        self._frameTillNextSubSpawn -= 1
        
        if self._frameTillNextSharkSpawn <= 0:
            self._spawn(self.enemy_dict['shark'])
            self._frameTillNextSharkSpawn = randint(*self._frameNextSharkBounds)
        
        if self._frameTillNextSubSpawn  <= 0:
            self._spawn(self.enemy_dict['submarine'])
            self._frameTillNextSubSpawn  = randint(*self._frameNextSubBounds)
        
        if self._enemies_spawned > 0:
            for enemy in self._active_enemies:
                enemy_despawned = False
                if enemy.isAlive:
                    
                    if (enemy.x <= enemy.width and enemy.direction == Direction.LEFT) or\
                        enemy.x >= 64.0-enemy.width and enemy.direction == Direction.RIGHT:
                        enemy_despawned = True
                    elif enemy.collide(player):
                        return GameEvent.LOSE
                    elif enemy.collide(bullet):
                        enemy_despawned = True
                        event = GameEvent.ENEMY_SHOT
                    else:  
                        if enemy.enemyType == 'submarine':
                        
                            if not enemy.hasFired  and\
                            enemy.y - 1  <= player.y and\
                            enemy.y+enemy.height+1 >= player.y and\
                            enemy.x >=0 and enemy.x <= 64:

                                if (enemy.direction == Direction.RIGHT and enemy.x < player.x) or\
                                (enemy.direction == Direction.LEFT and enemy.x > player.x):
                                    self.fireBullet(enemy)

                    if enemy_despawned:
                        enemy.isAlive = False
                        self._enemies_spawned = self._enemies_spawned - 1
                        self.enemy_dict[enemy.enemyType].append(enemy)
                        self._active_enemies.remove(enemy)
                    else:
                        enemy.update()

        return event