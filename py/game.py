from abc import ABC, abstractmethod
from .game_entity import GameEntity
import numpy as np
from math import ceil, floor
from numba import jit

@jit(nopython=True)
def _fillwithzero(buffer):
    buffer.fill(0)


@jit(nopython=True)
def _setBufferValue(buffer, x, y, width, height, color):
    x = np.floor(x)
    y = np.floor(y)
    width = np.ceil(width)
    height = np.ceil(height)
    buffer[y:y+height, x:x+width].fill(color)

@jit(nopython=True)
def _renderPixels(buffer, x, y, width, height, color):
    buffer.fill(0)
    x = np.floor(x)
    y = np.floor(y)
    width = np.ceil(width)
    height = np.ceil(height)
    for i in range(len(x)):
        buffer[y[i]:y[i]+height[i], x[i]:x[i]+width[i]].fill(color[i])


class Game:

    def __init__(self):

        self.screen_width = 64
        self.screen_height = 64
        self._gameEntities = []

    @property
    def gameEntities(self):
        return self._gameEntities

    @gameEntities.setter
    def gameEntities(self, _gameEntities):
        self._gameEntities = _gameEntities    

    @abstractmethod
    def step(self, action):
        pass

    def renderPixels(self, buffer):
        _fillwithzero(buffer)
        #buffer.fill(0)
        '''
        count = len(self.gameEntities)
        x = np.zeros(count,dtype=float)
        y = np.zeros(count,dtype=float)
        height = np.zeros(count,dtype=int)
        width = np.zeros(count,dtype=int)
        color = np.zeros(count,dtype=int)

        for i in range(count):
            x[i] = self.gameEntities[i].x
            y[i] = self.gameEntities[i].y
            width[i] = self.gameEntities[i].width
            height[i] = self.gameEntities[i].height
            color[i] = self.gameEntities[i].color
        _renderPixels(buffer, x,y,width,height,color)
        '''
        
        for gameEntity in self.gameEntities:
            
            x = gameEntity.x
            y = gameEntity.y
            height = gameEntity.height
            width  = gameEntity.width
            color  = gameEntity.color
            _setBufferValue(buffer,x,y,width,height,color)
            '''
            x = floor(gameEntity.x)
            y = floor(gameEntity.y)
            height = ceil(gameEntity.height)
            width  = ceil(gameEntity.width)
            color  = gameEntity.color
            buffer[y:y+height, x:x+width].fill(color)
            '''

        
    
    @abstractmethod
    def legalActions(self):
        pass

