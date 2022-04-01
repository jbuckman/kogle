from .game_entity import GameEntity
from random import randint

class Ball(GameEntity):

    def __init__(self, _color, _width, _height, _x, _y):
        super().__init__(_color, _width, _height, _x, _y)
        self._vx = randint(30,60)
        self._vy = 60

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