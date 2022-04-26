from py.cgame_entity import CGameEntity
from random import random, randint
from py.colors import *
from py.seaquest.game_event import GameEvent

MAX_DIVERS_SPAWNED = 2
MAX_DIVERS_SAVED = 6
DIVER_SPEED = 0.5
class DiverManager:

    def __init__(self):
        self._frameNextDiverBounds = [40, 60]
        self._frameTillNextSpawn = randint(*self._frameNextDiverBounds)
        self._divers = []

        self._divers_spawned = 0
        self._divers_saved_count  = 0

        for _ in range(MAX_DIVERS_SPAWNED):
            self._divers.append(CGameEntity(1, 2, 32, 32, BORING_COLORS[3], False))

        self._divers_saved = []

        for i in range(MAX_DIVERS_SAVED):
            self._divers_saved.append(CGameEntity(1, 2, 3+(i*2), 1, WHITE, False))


    @property
    def spawnRate(self):
        return self._spawnRate

    @spawnRate.setter
    def spawnRate(self, value):
        self._spawnRate = value

    @property
    def divers(self):
        return self._divers

    @divers.setter
    def divers(self, value):
        self._divers = value

    @property
    def divers_saved(self):
        return self._divers_saved

    @divers_saved.setter
    def divers_saved(self, value):
        self._divers_saved = value


    def retrieveOxygen(self):
        if self._divers_saved_count == 0:
            return GameEvent.LOSE
        elif self._divers_saved_count == MAX_DIVERS_SAVED:
            for diver in self._divers_saved:
                diver.isAlive = False
            self._divers_saved_count = 0 
            return GameEvent.NEXT_LEVEL
        else:
            self._divers_saved_count -= 1
            self._divers_saved[self._divers_saved_count].isAlive = False
            return GameEvent.RESCUE


    def update(self, player):

        self._frameTillNextSpawn -= 1
        
        if self._frameTillNextSpawn <= 0 and \
            self._divers_spawned == 0 and \
            self._divers_saved_count < MAX_DIVERS_SAVED:

            if self._divers_saved_count < MAX_DIVERS_SAVED-1:
                self._divers_spawned = randint(1,2)
            else:
                self._divers_spawned = 1

            for i in range(self._divers_spawned):

                self._divers[i].isAlive = True
                self._divers[i].y = randint(6, 55)
                left = random()

                if left < 0.5:    
                    self._divers[i].x = 0
                    self._divers[i].vx = DIVER_SPEED
                else:
                    self._divers[i].x = 62
                    self._divers[i].vx = -DIVER_SPEED

        
        if self._divers_spawned > 0:
            for diver in self._divers:
                if diver.isAlive:
                    if (diver.vx == DIVER_SPEED and diver.x >= 63) or \
                    (diver.vx == -DIVER_SPEED and diver.x <= 1) :
                        
                        diver.isAlive = False
                        self._divers_spawned -= 1
                    elif player.collide(diver):
                        
                        diver.isAlive = False
                        self._divers_saved[self._divers_saved_count].isAlive = True
                        self._divers_saved_count += 1
                        if self._divers_spawned > 0:
                            self._divers_spawned -= 1
                        else:
                            self._frameTillNextSpawn = randint(*self._frameNextDiverBounds)
                            break
                    else:
                        diver.update()

            if self._divers_spawned == 0:
                self._frameTillNextSpawn = randint(*self._frameNextDiverBounds)
