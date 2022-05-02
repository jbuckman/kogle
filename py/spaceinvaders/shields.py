from py.cgame_entity import CGameEntity
from py.colors import *

SHIELD_DIM = 3
class ShieldBlock(CGameEntity):
    def __init__(self, _x, _y):
        super().__init__(SHIELD_DIM, SHIELD_DIM, _x, _y, BORING_COLORS[3])
        self.reset()

    def hit(self):
        self.health -=1
        self.color = BORING_COLORS[self.health]
        if self.health == 0:
            self.isAlive = False

    def reset(self):
        self.health = 3 
        self.color = BORING_COLORS[self.health]
        self.isAlive = True

class ShieldManager:

    def __init__(self):
        self._shields = []
        for i in range(4):
            self._shields.extend([ShieldBlock(8+SHIELD_DIM*j+i*14, 52) for j in range(2)])

    @property
    def shields(self):
        return self._shields

    def reset(self):
        for shieldBlock in self._shields:
            shieldBlock.reset()
