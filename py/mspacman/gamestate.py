from enum import IntEnum

class GameState(IntEnum):
    NOTHING = 0
    GHOST_KILLED = 1
    GAME_OVER = 2
