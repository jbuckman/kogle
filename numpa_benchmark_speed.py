import random, time
from kogle.kogle import Kogle

def make_env():
    return Kogle(game_name="PyBreakout")

#Numpa Just-in-time-compiling take a bit for to initialize and cache machine code
env = make_env()
env.step(random.choice(env.legal_action_list))
print("Numpa Initialized")


env = None
_t = time.time()
frames = 0
while frames < 10000:
    if env is None or env.terminated:
        env = make_env()
    else:
        env.step(random.choice(env.legal_action_list))
    frames += 1
tt = time.time() - _t

print(f"Time {tt}, Frames {frames}, FPS {frames / tt}")