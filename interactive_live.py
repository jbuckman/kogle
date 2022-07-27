import sys
from kogle.kogle import *
import pygame
import numpy as np

def act(action):
    global score
    global env
    global contine_looping
    reward = env.step(action)
    score += reward
    if env.terminated: 
        contine_looping = False
        print(f"Score: {score}\n")
        pygame.quit()
        sys.exit()

def get_observation():
    global env
    new_observation = env.observation.repeat(8, 0).repeat(8, 1).T
    return np.repeat(new_observation[:, :, np.newaxis], 3, axis=2)

env = Kogle(game_name=sys.argv[1])

max_frame_rate = 30
if len(sys.argv) > 2 and sys.argv[2].isdigit():
    max_frame_rate = int(sys.argv[2])

score = 0
contine_looping = True
action_keys = {}
action_keys[80] = LEFT
action_keys[79] = RIGHT
action_keys[82] = UP
action_keys[81] = DOWN
action_keys[44] = FIRE
pygame.init()
clock = pygame.time.Clock()
screen = pygame.display.set_mode((512, 512))
next_action = NOOP

while contine_looping:   

    for event in pygame.event.get(): 
        if event.type == pygame.QUIT:
            contine_looping = False
        else:
            started = True
            keys = pygame.key.get_pressed()
            key_codes = np.where(keys)[0]
            
            if len(key_codes) == 1:
                next_action = action_keys[key_codes[0]]
            elif len(key_codes) == 2:
                key_codes = key_codes[key_codes != next_action]
                next_action = action_keys[key_codes[0]]
            else:
                next_action = NOOP
        

    act(next_action) 
    
    pygame.display.update()
    clock.tick(max_frame_rate)

    surface = pygame.surfarray.make_surface(get_observation())
    screen.blit(surface, (0,0))
    
  
    