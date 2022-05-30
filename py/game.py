from abc import ABC, abstractmethod
from py.cgame_entity import CGameEntity
from py.cgame_entity_group import CGameEntityGroup
#from .game_entity import GameEntity
from .cgame_entity import CGameEntity
from .cgame_entity_group import CGameEntityGroup
from py.cgame_entity import CGameEntity
from py.cgame_entity_group import CGameEntityGroup
import numpy as np


class Game:

    def __init__(self):

        self.screen_width = 64
        self.screen_height = 64
        self._gameEntityGroup = CGameEntityGroup()

    @property
    def gameEntityGroup(self):
        return self._gameEntityGroup

    def setDefaultBuffer(self, buffer):
        self._gameEntityGroup.setDefaultBuffer(buffer)

    @gameEntityGroup.setter
    def gameEntities(self, _gameEntities):
        self._gameEntityGroup = _gameEntities

    def step(self, action):
        pass

    def renderPixels(self, buffer):
        self.gameEntityGroup.render(buffer)
 
    @abstractmethod
    def legalActions(self):
        pass

