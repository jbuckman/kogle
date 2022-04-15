from py.cgame_entity import CGameEntity
from py.colors import *
from random import random, randint, uniform
from enum import IntEnum
from collections import deque

class RockSize(IntEnum):
    SMALL = 1
    MEDIUM = 2
    LARGE = 3

class Rock(CGameEntity):

    def __init__(self, rockSize, _x, _y):
        super().__init__(2**rockSize, 2**rockSize, _x, _y, BORING_COLORS[1], False)
        self._size = rockSize

    @property
    def size(self):
        return self._size

    def spawn(self, _x, _y, _vx, _vy):
        self.x = _x
        self.y = _y
        self.vx = _vx
        self.vy = _vy
        self.isAlive = True
        

class RockManager:

    def __init__(self, _spawnRate = 50):
        self._spawnRate = _spawnRate
        self._frameTillNextSpawn = _spawnRate
        self._rockQueues = []
        self._activeRocks = []
        self._rocks = []

        for rockSize in RockSize:
            rockList = [Rock(rockSize, 33, 33) for _ in range(2**(5-rockSize))] 
            rockQueue = deque(rockList)
            self._rockQueues.append(rockQueue)
            self._rocks.extend(rockList)

    
    @property
    def spawnRate(self):
        return self._spawnRate


    @spawnRate.setter
    def spawnRate(self, value):
        self._spawnRate = value

    @property 
    def activeRocks(self):
        return self._activeRocks

    @property 
    def rocks(self):
        return self._rocks

    def update(self):
        self._frameTillNextSpawn = self._frameTillNextSpawn - 1

        if self._frameTillNextSpawn <= 0:
            i = randint(0, 2)
            if len(self._rockQueues[i]) > 0:
                rock = self._rockQueues[i].popleft()
                random_num = random()
                if random_num <= 0.25:
                    rock.spawn(
                        randint(rock.width+1, 32-rock.width), 
                        rock.height+1, 
                        uniform(0.1, 0.3), 
                        uniform(0.1, 0.3)
                    )
                elif random_num <= 0.5:
                  rock.spawn(
                        randint(32, 64-rock.width), 
                        rock.height+1, 
                        -uniform(0.1, 0.3), 
                        uniform(0.1, 0.3)
                    )      
                elif random_num <= 0.75:      
                    rock.spawn(
                        randint(rock.width+1, 32-rock.width),
                        64-rock.height-1,
                        -uniform(0.1, 0.3), 
                        uniform(0.1, 0.3)
                    )   
                else:
                    rock.spawn(
                        randint(32, 64-rock.width-1),
                        64-rock.height-1,
                        -uniform(0.1, 0.3), 
                        -uniform(0.1, 0.3)
                    ) 

                self._activeRocks.append(rock)
                self._frameTillNextSpawn = self._spawnRate
        
        for rock in self._activeRocks:
            rock.update()


    def kill(self, rock):
        
        rock.isAlive = False
        self._rockQueues[rock.size-1].append(rock)
        self._activeRocks.remove(rock)
        

        if rock.size > 1 and len(self._rockQueues[rock.size-2]) >= 2:
            
            debris1 = self._rockQueues[rock.size-2].popleft()
            debris1.spawn(rock.x, rock.y, -rock.vx, -rock.vy)
            debris2 = self._rockQueues[rock.size-2].popleft()
            debris2.spawn(rock.x, rock.y, rock.vx, -rock.vy)

            self._activeRocks.append(debris1)
            self._activeRocks.append(debris2)
     
                




            



