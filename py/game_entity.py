from pygame.sprite import Sprite
from pygame.draw import rect
from pygame import Surface
from .colors import BLACK
class GameEntity(Sprite):

    def __init__(self, _color, _width, _height, _x, _y):
        super().__init__()
        self._width  = _width
        self._height = _height
        self.image = Surface([_width, _height])
        self.image.fill(BLACK)
        self.image.set_colorkey(BLACK)
        rect(self.image, _color, [0, 0, _width, _height])
        
        self.rect = self.image.get_rect()
        self.rect.x = _x
        self.rect.y = _y


    @property
    def width(self):
        return self._width

    @property
    def height(self):
        return self._height

    @property
    def x(self):
        return self.rect.x

    @x.setter
    def x(self, _x):
        self.rect.x = _x

    @property
    def y(self):
        return self.rect.y

    @y.setter
    def y(self, _y):
        self.rect.y = _y