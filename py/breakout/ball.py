from ..game_entity import GameEntity
from random import randint

class Ball(GameEntity):

    def __init__(self, _width, _height, _x, _y, _color, _isAlive=True):
        super().__init__(_width, _height, _x, _y, _color, _isAlive)
        self._vx = 0.5
        self._vy = 0.5

    @property
    def vx(self):
        return self._vx

    @vx.setter
    def vx(self, _vx):
        self._vx = _vx

    @property
    def vy(self):
        return self._vy

    @vy.setter
    def vy(self, _vy):
        self._vy = _vy