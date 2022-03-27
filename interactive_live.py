import tkinter as tk
import numpy as np
from kogle.kogle import *
import asyncio
import time
import keyboard

def _photo_image(image: np.ndarray):
    height, width = image.shape
    data = f'P5 {width} {height} 255 '.encode() + image.astype(np.uint8).tobytes()
    return tk.PhotoImage(width=width, height=height, data=data, format='PPM')

env = Kogle(game_name="Spaceinvaders")
score = 0
root = tk.Tk()

img = _photo_image(env.observation.repeat(8,0).repeat(8,1))
panel = tk.Label(root, image=img)
panel.pack(side="bottom", fill="both", expand="yes")

def act(action):
    global score
    global env
    reward = env.step(action)
    score += reward
    if reward != 0: print(f"{reward:+}")
    if env.terminated: root.destroy(); print(f"Score: {score}, Timesteps: {env.steps}"); return
    img = _photo_image(env.observation.repeat(8, 0).repeat(8, 1))
    panel.configure(image=img)
    panel.image = img

keys = {
    'space': FIRE, 
    'up arrow':UP, 
    'left arrow': LEFT, 
    'right arrow': RIGHT, 
    'down arrow': DOWN
}

async def game_loop(framerate_limit=30):
    loop = asyncio.get_event_loop()
    next_frame_target = 0.0
    limit_frame_duration = (1.0 / framerate_limit)
    while True:
        this_frame = time.time()
        delay = next_frame_target - this_frame
        if delay > 0:
            await asyncio.sleep(delay)
        next_frame_target = this_frame + limit_frame_duration
        
        next_action = NOOP

        for key in keys.keys():
            if keyboard.is_pressed(key):
                next_action = keys[key]
                break
                
        await loop.create_task(handle_update(next_action))

async def handle_update(action):
    act(action)
    root.update()
    
asyncio.run(game_loop())