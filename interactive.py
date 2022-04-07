import tkinter as tk
from kogle.kogle import *
import sys

def _photo_image(image: np.ndarray):
    height, width = image.shape
    data = f'P5 {width} {height} 255 '.encode() + image.astype(np.uint8).tobytes()
    return tk.PhotoImage(width=width, height=height, data=data, format='PPM')

env = Kogle(game_name=sys.argv[0])
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

root.bind("<z>", lambda event: act(NOOP))
root.bind("<space>", lambda event: act(FIRE))
root.bind("<Up>", lambda event: act(UP))
root.bind("<Down>", lambda event: act(DOWN))
root.bind("<Left>", lambda event: act(LEFT))
root.bind("<Right>", lambda event: act(RIGHT))
root.bind("<w>", lambda event: act(UPFIRE))
root.bind("<s>", lambda event: act(DOWNFIRE))
root.bind("<a>", lambda event: act(LEFTFIRE))
root.bind("<d>", lambda event: act(RIGHTFIRE))

root.mainloop()
