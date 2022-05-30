from py.cgame_entity import CGameEntity
from py.actions import Action
import numpy as np
from py.colors import *
from math import ceil

class Player(CGameEntity):

    def __init__(self, _x, _y):
        super().__init__(4, 4, _x, _y, BORING_COLORS[5])
        self._lastAction = Action.UP

    @property
    def lastAction(self):
        return self._lastAction

    def move(self, action, amount, levelLayout):

        x1 = int(min(max(self.x, 0), 63))
        y1 = int(min(max(self.y, 0), 63))
        x2 = int(min(max(self.width+x1, 0), 63))
        y2 = int(min(max(self.height+y1, 0), 63))
        nextStep = ceil(amount)
        
        if action == Action.UP and np.all(levelLayout[y1-nextStep, x1:x2]==0):
            self.vy = -amount
            self.vx = 0
            self._lastAction = action

        elif action == Action.DOWN and np.all(levelLayout[y2, x1:x2]==0):
            self.vy = amount
            self.vx = 0
            self._lastAction = action

        elif action == Action.LEFT and np.all(levelLayout[y1:y2, x1-nextStep]==0):
            self.vx = -amount
            self.vy = 0
            self._lastAction = action

        elif action == Action.RIGHT and np.all(levelLayout[y1:y2, x2]==0):                
            self.vx = amount
            self.vy = 0
            self._lastAction = action

        else:
            self.vy = 0
            self.vx = 0

    

    