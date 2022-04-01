from abc import ABC, abstractmethod
from .colors import BLACK, RED 
from .game_entity import GameEntity
import numpy as np
import pygame
class Game(ABC):

    def __init__(self):

        self.dt = 1
        self.pygame = pygame
        self.pygame.init()
        self.screen_width = 640
        self.screen_height = 640
        self.screen = pygame.display.set_mode((self.screen_width, self.screen_height))
        self.gameEntities = pygame.sprite.Group()
    
    @abstractmethod
    def update(self, action):
        pass

    def step(self, action):
        self.screen.fill(BLACK)
        score, isDone = self.update(action)
        self.gameEntities.update()
        self.gameEntities.draw(self.screen)
        pygame.display.flip()
        return score, isDone
       
    def renderPixels(self, buffer):
        buffer = np.zeros((64,64))

        for gameEntity in self.gameEntities:
            x = int(gameEntity.rect.x/10)   
            y = int(gameEntity.rect.y/10)
            height = int(gameEntity.rect.height/10)
            width  = int(gameEntity.rect.width/10)

            if height > 0 and width > 0:
          
                buffer[y:y+height,x:x+width] = np.ones((height,width))

    @abstractmethod
    def legalActions(self):
        pass

    def reset(self):
        self.gameEntities.empty()

        
    def createNewEntity(self, _color, _width, _height, _x, _y, game_entity_type=GameEntity):
        gameEntity = game_entity_type(_color, _width, _height, _x, _y)
        self.gameEntities.add(gameEntity)
        return gameEntity

    def setDeltaT(self, dt):
        self.dt = dt