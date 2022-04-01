import numpy as np
from kogle.kogle import *
import pygame
import asyncio
import time
import statistics
import time

frame_rate = 60
clock = pygame.time.Clock()
env = Kogle(game_name="PyBreakout", frame_rate=frame_rate)
score = 0

contine_looping = True
ticks = []
action_keys = {}
action_keys[pygame.K_LEFT] = LEFT
action_keys[pygame.K_RIGHT] = RIGHT

def act(action):
    global score
    global env
    global contine_looping
    reward = env.step(action)
    score += reward
    if env.terminated: 
        contine_looping = False
        print(f"Score: {score}")
        print(f"Average time between frame: {statistics.mean(ticks)} ms")
        pygame.quit()
  
def game_loop(frame_rate):

    global contine_looping
    global ticks

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
        tick = clock.tick(frame_rate)
        ticks.append(tick)

game_loop(frame_rate)