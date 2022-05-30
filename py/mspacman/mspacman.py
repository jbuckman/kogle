from random import random
from py.game import Game
from py.cgame_entity import CGameEntity
from py.colors import *
from py.actions import Action
import numpy as np
from py.mspacman.levels import LEVEL_1_16,LEVEL_1_64
from py.mspacman.pellets import PelletManager
from py.mspacman.player import Player
from py.mspacman.ghosts import GhostManager, GhostHouse
from py.mspacman.gamestate import GameState

class MsPacman(Game):

    def __init__(self):
        super().__init__()

        self.setDefaultBuffer(
            np.array(LEVEL_1_64*BORING_COLORS[5], dtype=np.uint8)
        )

        self.levelLayout = LEVEL_1_64
        self.pelletManager = PelletManager()
        self.player = Player(28, 56)
        self.ghostHouse = GhostHouse(28, 28)
        self.ghostManager = GhostManager(self.ghostHouse)
        self.isEnergized = False
        self.energizedTimer = 0
        self.energizedDuration = 30
        self.pelletConsumed = 0
        self.pelletAmount = len(self.pelletManager.pellets)
        self.ghostKillStreak = 0 

        self.lastAction = None
 
        self.gameEntities.extend([
            self.player,
            *self.ghostManager.ghosts,
            *self.pelletManager.pellets,
            *self.pelletManager.powerPellets
        ])

    def nextLevel(self):
         self.isEnergized = False
         self.energizedTimer = 0
         self.ghostKillStreak = 0 
         self.lastAction = None
         self.player.x = 28
         self.player.y = 56

        
    def step(self, action):
        score = 0
        if action in [Action.UP, Action.DOWN, Action.RIGHT, Action.LEFT]:
            self.player.move(action, 1, self.levelLayout)
            self.lastAction = action
            self.player.update()
        elif self.lastAction == None:
            pass
        else:
            self.player.move(self.lastAction, 1, self.levelLayout)
            self.player.update()

        for pellet in self.pelletManager.pellets:
            if pellet.isAlive and self.player.collide(pellet):
                self.pelletManager.pellets.remove(pellet)
                self.gameEntities.remove(pellet)
                self.pelletConsumed += 1
                score += 10
                break
        
        if self.isEnergized:
            if self.energizedTimer < self.energizedDuration:
                self.energizedTimer += 1
            else:
                self.energizedTimer = 0
                self.isEnergized = False
                self.ghostKillStreak = 0

        for powerPellet in self.pelletManager.powerPellets:
            if powerPellet.isAlive:
                if powerPellet.collide(self.player):
                    powerPellet.isAlive = False
                    self.isEnergized = True
                    score += 50
                else:
                    powerPellet.update()

        gameState = self.ghostManager.update(self.player, self.isEnergized, self.pelletConsumed)
        
        if gameState == GameState.GHOST_KILLED:
            score += 200*(2**(self.ghostKilledStreak))
            self.ghostKilledStreak += 1
        elif gameState == GameState.GAME_OVER:
            return (score, True)


        return (score, False)

    def legalActions(self):
        return (True, False, True, True, True, True)