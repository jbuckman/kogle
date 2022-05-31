from tkinter import RIGHT
from py.cgame_entity import CGameEntity
from py.colors import *
from py.mspacman.levels import level1
from enum import IntEnum
from abc import ABC, abstractmethod
import numpy as np
from math import sqrt, ceil
from py.actions import Action
from random import randint, choice
from py.mspacman.gamestate import GameState

class GhostState(IntEnum):
    SCATTER = 0
    CHASE = 1
    FRIGHTENDED = 2
    NOT_ACTIVE = 3
    DEAD = 4


class Ghost(CGameEntity, ABC):

    def __init__(self, _x, _y, _state, _limit):
        super().__init__(4, 4, _x, _y, BORING_COLORS[4])
        self._levelLayout = level1
        self._lastMove = None
        self._timer = 0
        self._state = GhostState.SCATTER
        self._lastState = None
        self._scatter_duration = 10
        self._chase_duration = 40
        self._state = _state
        self._normalSpeed = 0.75
        self._frightenedSpeed = 0.5
        self._initialPosition = [_x, _y]
        self._reviveTimer = 0
        self._reviveDuration = 50
        self._limit = _limit



    @property
    def state(self):
        return self._state

    @state.setter
    def state(self, value):
        self._state = value

    @property
    def limit(self):
        return self._limit

    @state.setter
    def limit(self, value):
        self._limit = value

    @property
    def moveAmount(self):
        return self._moveAmount

    @moveAmount.setter
    def moveAmount(self, value):
        self._moveAmount = value
        self.unitVectors = ((0, -self._moveAmount), (-self._moveAmount, 0), (0, self._moveAmount), (self._moveAmount, 0))

    @property
    def levelLayout(self):
        return self._levelLayout

    @abstractmethod
    def scatterTarget(self):
        pass

    @abstractmethod
    def chase(self, player, **kwargs):
        pass

    @levelLayout.setter
    def levelLayout(self, value):
        self._levelLayout = value

    def update(self, player, isFrightened, pelletsConsumed, **kwargs):
        

        if self._state == GhostState.NOT_ACTIVE:
            
            if pelletsConsumed > self._limit:
                self._state = GhostState.SCATTER
            else:
                return GameState.NOTHING

        if self._state == GhostState.DEAD:
            self._reviveTimer += 1
            if self._reviveTimer < self._reviveDuration:
                return GameState.NOTHING
            else:
                self._state = GhostState.SCATTER 
        
        if self.collide(player):
            if isFrightened or self._state == GhostState.FRIGHTENDED:
                self._reviveTimer = 0
                self._state = GhostState.DEAD
                self.x = self._initialPosition[0]
                self.y = self._initialPosition[1]
                return GameState.GHOST_KILLED
            else: 
                return GameState.GAME_OVER

       

        if self._state == GhostState.FRIGHTENDED and isFrightened:
            self.randomMove()
            return GameState.NOTHING
        
        if self._state == GhostState.FRIGHTENDED and not isFrightened:
            self._state = self._lastState

        if isFrightened:
            self._lastState = self._state
            self._state = GhostState.FRIGHTENDED
            
            if self._lastMove == Action.UP:
                self.y += self._frightenedSpeed
                self._lastMove = Action.DOWN
            elif self._lastMove == Action.LEFT:
                self.x += self._frightenedSpeed
                self._lastMove = Action.RIGHT
            elif self._lastMove == Action.DOWN:
                self.y -= self._frightenedSpeed
                self._lastMove = Action.UP
            elif self._lastMove == Action.RIGHT:
                self.x -= self._frightenedSpeed
                self._lastMove = Action.LEFT

            
        elif self._state == GhostState.SCATTER: 
            self._timer += 1
            self.scatter()
            if self._timer > self._scatter_duration:
                self._timer = 0
                self._state = GhostState.CHASE
        elif self._state == GhostState.CHASE:
            self._timer += 1
            self.chase(player, **kwargs)
            if self._timer > self._chase_duration:
                self._timer = 0
                self._state = GhostState.SCATTER

        return GameState.NOTHING


    def scatter(self):
        self.followTarget(self.scatterTarget())

    def randomMove(self):
        
        moves = np.zeros(4)

        x1 = int(min(max(self.x, 0), 63))
        y1 = int(min(max(self.y, 0), 63))
        x2 = int(min(max(self.width+x1, 0), 63))
        y2 = int(min(max(self.height+y1, 0), 63))
        nextStep = ceil(self._frightenedSpeed)
    
        if np.all(self._levelLayout[y1-nextStep, x1:x2]==0):
            moves[0] += 1
            if self._lastMove == Action.UP:
                moves[0] += 4

        if np.all(self._levelLayout[y1:y2, x1-nextStep]==0):
            moves[1] += 1
            if self._lastMove == Action.LEFT:
                moves[1] += 4

        if np.all(self._levelLayout[y2, x1:x2]==0):
            moves[2] += 1
            if self._lastMove == Action.DOWN:
                moves[2] += 4
    
        if np.all(self._levelLayout[y1:y2, x2]==0):
            moves[3] += 1
            if self._lastMove == Action.RIGHT:
                moves[3] += 4
        
        moves = moves/np.sum(moves)
        index = np.random.choice(range(4), p=moves)
        
        if index == 0:
            self.y -= self._frightenedSpeed
            self._lastMove = Action.UP
        elif index == 1:
            self.x -= self._frightenedSpeed
            self._lastMove = Action.LEFT
        elif index == 2:
            self.y += self._frightenedSpeed
            self._lastMove = Action.DOWN
        elif index == 3:
            self.x += self._frightenedSpeed
            self._lastMove = Action.RIGHT


    def followTarget(self, target):
        # UP, LEFT, DOWN, RIGHT
        dist = [999.0]*4

        x1 = int(min(max(self.x, 0), 63))
        y1 = int(min(max(self.y, 0), 63))
        x2 = int(min(max(self.width+x1, 0), 63))
        y2 = int(min(max(self.height+y1, 0), 63))
        nextStep = ceil(self._normalSpeed)
        # UP
        if self._lastMove != Action.DOWN and np.all(self._levelLayout[y1-nextStep, x1:x2]==0):
            dist[0] = self.euclidianDistance(self.x,self.y-self._normalSpeed,*target)

        # LEFT

        if self._lastMove != Action.RIGHT and np.all(self._levelLayout[y1:y2, x1-nextStep]==0):
            dist[1] = self.euclidianDistance(self.x-self._normalSpeed,self.y,*target)

        # DOWN

        if self._lastMove != Action.UP and np.all(self._levelLayout[y2, x1:x2]==0):
            dist[2] = self.euclidianDistance(self.x,self.y+self._normalSpeed,*target)

        # RIGHT

        if self._lastMove != Action.LEFT and np.all(self._levelLayout[y1:y2, x2]==0):
            dist[3] = self.euclidianDistance(self.x+self._normalSpeed,self.y,*target)

        minimum = self.argmin(dist)

        if minimum == 0:
            self.y -= self._normalSpeed
            self._lastMove = Action.UP
        elif minimum == 1:
            self.x -= self._normalSpeed
            self._lastMove = Action.LEFT
        elif minimum == 2:
            self.y += self._normalSpeed
            self._lastMove = Action.DOWN
        elif minimum == 3:
            self.x += self._normalSpeed
            self._lastMove = Action.RIGHT

        
    def argmin(self, list):

        minValue = 999.0
        minIndex = 0
        for idx, value in enumerate(list):
            if value < minValue:
                minValue = value
                minIndex = idx
        
        return minIndex

    def euclidianDistance(self, x1, y1, x2, y2):
        return sqrt((x2-x1)**2+(y2-y1)**2)

class Blinky(Ghost):
    def __init__(self, _x, _y, _state):
        super().__init__(_x, _y, _state, 0)

    def scatterTarget(self):
        return [56, -4]

    def chase(self, player, **kwargs):
        self.followTarget([player.x, player.y])

class Pinky(Ghost):
    def __init__(self, _x, _y, _state):
        super().__init__(_x, _y, _state, 0)

    def scatterTarget(self):
        return [56, 64]

    def chase(self, player, **kwargs):
        if player.lastAction == Action.UP:
            self.followTarget([player.x, player.y-4])
        elif player.lastAction == Action.LEFT:
            self.followTarget([player.x-4, player.y])
        elif player.lastAction == Action.RIGHT:
            self.followTarget([player.x+4, player.y]) 
        elif player.lastAction == Action.DOWN:
            self.followTarget([player.x, player.y+4]) 

class Inky(Ghost):
    def __init__(self, _x, _y, _state, _limit):
        super().__init__(_x, _y, _state, _limit)

    def scatterTarget(self):
        return [8, -4]

    def chase(self, player, **kwargs):
        blinky = kwargs['blinky']
        target = np.array([player.x, player.y])
        if player.lastAction == Action.UP:
            target[1] -= 2
        elif player.lastAction == Action.LEFT:
            target[0] -= 2
        elif player.lastAction == Action.RIGHT:
            target[0] += 2
        elif player.lastAction == Action.DOWN:
            target[1] += 2
        
        target = target-1*(np.array([blinky.x, blinky.y])  - target)
        self.followTarget([target[0], target[1]]) 


class Clyde(Ghost):
    def __init__(self, _x, _y, _state, _limit):
        super().__init__(_x, _y, _state, _limit)

    def scatterTarget(self):
        return [0, 64]

    def chase(self, player, **kwargs):

        distance = self.euclidianDistance(self.x,self.y, player.x, player.y)
        if distance > 8: 
            self.followTarget([player.x, player.y])
        else:
            self.followTarget(self.scatterTarget())

class GhostManager:
    def __init__(self, _ghostHouse):
        self._ghostHouse = _ghostHouse
        self.blinky  = Blinky(_ghostHouse.x,_ghostHouse.y+4, GhostState.SCATTER)
        self.pinky   = Pinky(_ghostHouse.x+4,_ghostHouse.y+4, GhostState.SCATTER)
        self.inky    = Inky(_ghostHouse.x,_ghostHouse.y, GhostState.NOT_ACTIVE, 30)
        self.clyde   = Clyde(_ghostHouse.x+4,_ghostHouse.y, GhostState.NOT_ACTIVE, 60)
        
        self._ghosts = [
            self.blinky, 
            self.pinky,
            self.inky,
            self.clyde
        ]


    @property
    def ghosts(self):
        return self._ghosts

    def update(self, player, frightened, pelletsConsumed):

        gameState = GameState.NOTHING
        for ghost in self._ghosts:
            nextGameSate = ghost.update(player, frightened, pelletsConsumed, blinky=self.blinky)
            if nextGameSate == GameState.GAME_OVER:
                return nextGameSate
            elif gameState != GameState.NOTHING:
                gameState = nextGameSate
        
        return gameState

class GhostHouse(CGameEntity):
    def __init__(self, _x, _y):
        super().__init__(8, 4, _x, _y, BORING_COLORS[1])
