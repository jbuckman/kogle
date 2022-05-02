from py.game import Game
from py.cgame_entity import CGameEntity
from py.colors import *
from py.actions import Action
from py.spaceinvaders.game_event import GameEvent
from py.spaceinvaders.shields import ShieldManager
from py.spaceinvaders.enemies import EnemyManager

MIN_PLAYER_X = 0
MAX_PLAYER_X = 59

class SpaceInvaders(Game):

    def __init__(self):
        super().__init__()
        self.player = CGameEntity(5, 3, 30, 60, WHITE)
        self.bullet = CGameEntity(1, 2, 30, 60, WHITE, False)
        self.shieldManager = ShieldManager()
        self.enemyManager = EnemyManager()

        self.gameEntities.extend([
            *self.enemyManager.enemies,
            *self.enemyManager.bullets,
            *self.shieldManager.shields,
            self.player,
            self.bullet 
        ])

    def nextLevel(self):
        self.enemyManager.nextLevel()
        self.shieldManager.reset()

        self.player.x = 30
        self.bullet.isAlive = False
        
    def step(self, action):
        score = 0
        self.player.vx = 0

        if action == Action.LEFT and self.player.x > MIN_PLAYER_X:
            self.player.vx = -1
            self.player.update()
        elif action == Action.RIGHT and self.player.x < MAX_PLAYER_X:
            self.player.vx = 1
            self.player.update()
        elif action == Action.FIRE and not self.bullet.isAlive:
            self.bullet.isAlive = True
            self.bullet.y = self.player.y-2
            self.bullet.x = self.player.x+self.player.width/2
            
        event = self.enemyManager.update(self.bullet)

        if event == GameEvent.GAME_OVER:
            return (0, True)
        elif event == GameEvent.ENEMY_SHOT:
            score += 10
        elif event == GameEvent.NEXT_LEVEL:
            score += 10
            self.nextLevel()
            return (score, False)

        for bullet in self.enemyManager.active_enemey_bullets:
            
            despawn = False
            for shield in self.shieldManager.shields:
                if shield.isAlive and self.bullet.collide(shield):
                    shield.hit()
                    self.bullet.isAlive = False
                if shield.isAlive and bullet.collide(shield):
                    despawn = True
                    shield.hit()
            
            if not despawn and bullet.collide(self.player):
                return (0, True)
            
            if not despawn and bullet.y >= 64:
                despawn = True
            
            if despawn:
                self.enemyManager.despawnBullet(bullet)
            else:
                bullet.update()

        if self.bullet.isAlive:
            self.bullet.vy = -3
            self.bullet.update()
            if self.bullet.y <= 0:
                self.bullet.isAlive = False
        return (score, False)

    def legalActions(self):

        return (True, True, False, False, True, True)