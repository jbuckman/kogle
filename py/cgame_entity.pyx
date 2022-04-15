from libc.math cimport fmax, fmin    
import numpy as np
cdef class CGameEntity:

    def __init__(self, _width, _height, _x, _y, _color, _isAlive = True):
        self._width = _width
        self._height = _height
        self._x = _x
        self._y = _y
        self._color = _color
        self._vx = 0
        self._vy = 0
        self._isAlive = _isAlive

    @property
    def width(self):
        return self._width

    @width.setter
    def width(self, value):
        self._width = value

    @property
    def height(self):
        return self._height

    @height.setter
    def height(self, value):
        self._height = value

    @property
    def x(self):
        return self._x

    @x.setter
    def x(self, value):
        self._x = value

    @property
    def y(self):
        return self._y

    @y.setter
    def y(self, value):
        self._y = value

    @property
    def vx(self):
        return self._vx

    @vx.setter
    def vx(self, value):
        self._vx = value

    @property
    def vy(self):
        return self._vy

    @vy.setter
    def vy(self, value):
        self._vy = value

    @property
    def color(self):
        return self._color

    @color.setter
    def color(self, value):
        self._color = value

    @property
    def isAlive(self):
        return self._isAlive

    @isAlive.setter
    def isAlive(self, value):
        self._isAlive = value


    @property
    def id(self):
        return self._id

    @id.setter
    def id(self, value):
        self._id = value

    def __eq__(self, other):
        return isinstance(other, CGameEntity) and self.id == (<CGameEntity>other).id
    
    def __ne__(self, other):
        return isinstance(other, CGameEntity) and self.id != (<CGameEntity>other).id

    cpdef void update(self):
        if self._isAlive:
            self._x = fmax(0, fmin(64 - self._width, self._x+self._vx))
            self._y = fmax(self._height, fmin(64 - self._height, self._y+self._vy))
    
    cpdef bool collide(self, CGameEntity other):
        if not self._isAlive:
            return False

        return(
            self._x < other._x + other._width  and
            self._x + self._width > other._x   and
            self._y < other._y + other._height and
            self._height + self._y > other._y
        )
     
