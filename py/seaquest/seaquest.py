from xmlrpc.client import TRANSPORT_ERROR
from py.game import Game
from py.cgame_entity import CGameEntity
from py.colors import *
from random import random, randint, uniform
from py.seaquest.divers import DiverManager
from py.seaquest.enemies import EnemyManager
from py.seaquest.game_event import GameEvent
from py.seaquest.direction import Direction
from py.actions import Action

MIN_PLAYER_X = 0
MAX_PLAYER_X = 60
MIN_PLAYER_Y = 3
MAX_PLAYER_Y = 57
class Seaquest(Game):

    def __init__(self):
        super().__init__()
        self.reset()
    
    def reset(self):

        self.diverManager =  DiverManager()
        self.enemyManager =  EnemyManager()
        self.air = CGameEntity(64, 6, 0, 0, BORING_COLORS[0])
        
        self.player = CGameEntity(4, 3, 32, 3, WHITE)
        self.bullet = CGameEntity(2, 1, 32, 3, WHITE, False)

        self.player_direction = Direction.RIGHT
        self.upForAir = True

        self.level = 0 
        self.points_from_enemy_shot = 20
        self.points_from_diver_saved = 50

        self.oxygenBar = CGameEntity(50, 2, 6, 60, BORING_COLORS[4])
        self.oxygenLevel = 200
    
        self.gameEntities.extend([
            self.air,
            *self.diverManager.divers_saved,
            self.player,
            self.bullet,
            self.oxygenBar, 
            *self.diverManager.divers,
            *self.enemyManager.enemies
        ])

    def fireBullet(self):
        self.bullet.y = self.player.y + self.player.height/2
        self.bullet.isAlive = True
        if self.player_direction == Direction.LEFT:
            self.bullet.x = self.player.x
            self.bullet.vx = -2
        else:
            self.bullet.x = self.player.x+self.player.width
            self.bullet.vx = 2    

    def nextLevel(self):
        self.level += 1
        self.points_from_enemy_shot = min(20 + 10*self.level, 90)
        self.upForAir = True
        self.enemyManager.nextLevel()
        self.resetOxygenBar()    
        self.bullet.isAlive = False
        self.player.x = 32
        self.player.y = 3
        self.player_direction = Direction.RIGHT
            
    def step(self, action):

        score = 0
        player_movement = True

        
        self.player.vx = self.player.vy = 0
        if action == Action.UP and self.player.y > MIN_PLAYER_Y:
            self.player.vy = -1
        elif action == Action.DOWN and self.player.y < MAX_PLAYER_Y:
            self.player.vy = 1
        elif action == Action.LEFT and self.player.x > MIN_PLAYER_X:
            self.player_direction = Direction.LEFT
            self.player.vx = -1
        elif action == Action.RIGHT and self.player.x < MAX_PLAYER_X:
            self.player_direction = Direction.RIGHT
            self.player.vx = 1
        elif action == Action.FIRE and not self.bullet.isAlive:
            self.fireBullet()
            player_movement = False
        elif action == Action.NO_ACTION:
            player_movement = False


        if player_movement:
            self.player.update()

        if self.bullet.isAlive:
            if self.bullet.x <=0 or self.bullet.x >= self.screen_width:
                self.bullet.isAlive = False
            else:
                self.bullet.update()

        enemy_event = self.enemyManager.update(self.player, self.bullet)

        if enemy_event == GameEvent.LOSE:
            return (score, True)
        elif enemy_event == GameEvent.ENEMY_SHOT:
          
            score += self.points_from_enemy_shot

        self.diverManager.update(self.player)
        if self.upForAir and self.player.y > 3:
            self.upForAir = False
        elif not self.upForAir and self.player.y <= 3:
            state = self.diverManager.retrieveOxygen()
            if state == GameEvent.RESCUE:
                self.upForAir = True
                score += self.rescueDivers(1)
                score += self.oxygenLevel
                self.resetOxygenBar()
            elif state == GameEvent.NEXT_LEVEL:
                score += self.rescueDivers(6)
                score += self.oxygenLevel
                self.nextLevel()
            elif state == GameEvent.LOSE:
                return (score, True)

        if not self.upForAir:
           self.updateOxygenBar()
        if self.oxygenLevel <= 0:
            return (score, True)

        return (score, False)

    def rescueDivers(self, diver_count):
        score = 0
        for _ in range(diver_count):
            score += self.points_from_diver_saved
            self.points_from_diver_saved = min(1000, self.points_from_diver_saved+50)
        return score

    def updateOxygenBar(self):
        self.oxygenLevel -= 1
        if self.oxygenLevel % 20 == 0:
            self.oxygenBar.width -= 5
        
    def resetOxygenBar(self):
        self.oxygenBar.width = 50
        self.oxygenLevel = 200

    def legalActions(self):
        return (True, True, True, True, True, True)

   