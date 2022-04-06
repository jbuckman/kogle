from itertools import count
from numba import jit

@jit(nopython=True)
def _collide(x1, y1, width1, height1, x2, y2, width2, height2):
    return  x1 < x2 + width2 and x1 + width1 > x2   and y1 < y2 + height2 and height1 + y1 > y2

class GameEntity:

    _counter = count(0)

    def __init__(self, _width, _height, _x, _y, _color, _isAlive=True):
        self._id = next(self._counter)
        self._width  = _width
        self._height = _height
        self._x = _x
        self._y = _y
        self._color = _color
        self._isAlive = True

    @property
    def id(self):
        return self._id

    @property
    def width(self):
        return self._width

    @property
    def height(self):
        return self._height

    @property
    def x(self):
        return self._x

    @x.setter
    def x(self, _x):
        self._x = _x

    @property
    def y(self):
        return self._y

    @y.setter
    def y(self, _y):
        self._y = _y

    @property
    def color(self):
        return self._color

    @color.setter
    def color(self, _color):
        self._color = _color

    @property
    def isAlive(self):
        return self._isAlive

    @isAlive.setter
    def isAlive(self, _isAlive):
        self._isAlive = _isAlive

    def collide(self, other):
        '''
        return _collide(
            self.x, self.y, self.width, self.height, 
            other.x, other.y, other.width, other.height
        )
        '''
        
        return(
            self.x < other.x + other.width  and
            self.x + self.width > other.x   and
            self.y < other.y + other.height and
            self.height + self.y > other.y
        )
        

    def __repr__(self):
        return str(self.id)

    def __eq__(self, other):
        return self.id == other.id

    def __ne__(self, other):
        return self.id != other.id

    def to_list(self):
        return (self.x, self.y, self.width, self.height, self.color)