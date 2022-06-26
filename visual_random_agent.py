import random, time, sys
from kogle.kogle import Kogle
import tkinter as tk
import numpy as np

def _photo_image(image: np.ndarray):
    height, width = image.shape
    data = f'P5 {width} {height} 255 '.encode() + image.astype(np.uint8).tobytes()
    return tk.PhotoImage(width=width, height=height, data=data, format='PPM')

def make_env():
    return Kogle(game_name=sys.argv[1])

root = tk.Tk()
env = make_env()
img = _photo_image(env.observation.repeat(8,0).repeat(8,1))
frames = 0
panel = tk.Label(root, image=img)
panel.pack(side="bottom", fill="both", expand="yes")


while frames < 10000:
    if env is None or env.terminated:
        env = make_env()
    time.sleep(0.05)
    env.step(random.choice(env.legal_action_list))
    img = _photo_image(env.observation.repeat(8,0).repeat(8,1))
    panel.configure(image=img)
    panel.image = img
    frames += 1
    root.update()