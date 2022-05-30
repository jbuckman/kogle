from py.cgame_entity import CGameEntity
from py.colors import *
from py.mspacman.levels import LEVEL_1_16
import numpy as np

class Pellet(CGameEntity):

    def __init__(self, _x, _y, _isPower = False):
        super().__init__(2, 2, _x, _y, BORING_COLORS[2])
        self._isPower = _isPower
        self.toggle = 0
    
    @property
    def isPower(self):
        return self._isPower

    def update(self):
        if self._isPower:
            self.toggle ^= 1
            self.color = BORING_COLORS[2+self.toggle*2]


class PelletManager:

    def __init__(self, _level=1):
        self._level = _level
        self.reset()

    def reset(self):
        self._pellets = []
        self._powerPellets = []
        self.spawnPellets()

    @property
    def pellets(self):
        return self._pellets

    @pellets.setter
    def pellets(self, value):
        self._pellets = value

    @property
    def powerPellets(self):
        return self._powerPellets

    @powerPellets.setter
    def powerPellets(self, value):
        self._powerPellets = value


    def spawnPellets(self):

        no_pellet_locations = []
        power_pellet_locations = []
        level_layout = None
        if self._level == 1:
            no_pellet_locations = [(7,7), (8,7), (7,14), (8,14)]
            power_pellet_locations = [(1, 1),(14,1),(1, 11),(14, 11)]
            level_layout = LEVEL_1_16

        for r, c in zip(*np.where(level_layout == 0)):

            if not (c, r) in no_pellet_locations:

                isPower = True if (c, r) in power_pellet_locations else False 
                if isPower:
                    self._powerPellets.append(Pellet(1+c*4, 1+r*4, True))
                else:
                    self._pellets.append(Pellet(1+c*4, 1+r*4))




