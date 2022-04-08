from libcpp cimport bool

cdef class CGameEntity:

    def __init__(self, _width, _height, _x, _y, _color):
        self._width = _width
        self._height = _height
        self._x = _x
        self._y = _y
        self._color = _color


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
    def color(self):
        return self._color

    @color.setter
    def color(self, value):
        self._color = value

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
    
    cpdef int collide(self, CGameEntity other):
        return(
            self._x < other._x + other._width  and
            self._x + self._width > other._x   and
            self._y < other._y + other._height and
            self._height + self._y > other._y
        )
     
