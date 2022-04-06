
import pygame

class Sprite(pygame.sprite.Sprite):

    def __init__(self, _gameEntity, scale = 10):
        super().__init__()

        self._gameEntity = _gameEntity
        self.scale = scale
        self.image = pygame.Surface([_gameEntity.width*self.scale, _gameEntity.height*self.scale])
        
        pygame.draw.rect(
            self.image, 
            [_gameEntity.color]*3, 
            [0, 0, _gameEntity.width*self.scale, _gameEntity.height*self.scale]
        )
        
        self.rect = self.image.get_rect()
        self.rect.x = _gameEntity.x*self.scale
        self.rect.y = _gameEntity.y*self.scale

    def update(self):
        self.x = self.gameEntity.x*self.scale
        self.y = self.gameEntity.y*self.scale

    def draw(self, screen):
        screen.blit(self.image, self.rect)

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