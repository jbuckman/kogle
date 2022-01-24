import random, time
from kogle.kogle import Kogle

def make_env():
    return Kogle(game_name="Breakout")

env = None
_t = time.time()
frames = 0
while frames < 10000:
    if env is None or env.terminated:
        env = make_env()
    else:
        env.step(random.choice(env.legal_action_set))
    frames += 1
tt = time.time() - _t
print(f"Time {tt}, Frames {frames}, FPS {frames / tt}")