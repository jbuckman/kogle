import sys
from kogle.kogle import *
import pygame
import time

def act(action):
    global score
    global env
    global contine_looping
    reward = env.step(action)
    score += reward
    if env.terminated: 
        contine_looping = False
        print(f"Score: {score}")
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
action_keys[pygame.K_LEFT] = LEFT
action_keys[pygame.K_RIGHT] = RIGHT
action_keys[pygame.K_UP] = UP
action_keys[pygame.K_DOWN] = DOWN
action_keys[pygame.K_SPACE] = FIRE
pygame.init()
clock = pygame.time.Clock()
screen = pygame.display.set_mode((512, 512))
next_action = NOOP

while contine_looping:   

    for event in pygame.event.get(): 
        if event.type == pygame.QUIT:
            contine_looping = False
        elif event.type == pygame.KEYDOWN:
            started = True
            keys = pygame.key.get_pressed()
            for key in action_keys:
                if event.key == key:
                    next_action = action_keys[key]
                    break
            
        elif event.type == pygame.KEYUP:
            next_action = NOOP

    act(next_action) 
    clock.tick(max_frame_rate)

    surface = pygame.surfarray.make_surface(get_observation())
    screen.blit(surface, (0,0))
    pygame.display.update()
    pygame.display.flip() 
  
    