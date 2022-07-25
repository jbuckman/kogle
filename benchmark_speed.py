import random, time, sys
from kogle.kogle import Kogle

def make_env():
    return Kogle(game_name=sys.argv[1])


env = None
_t = time.time()
obs = []
frames = 0
while frames < 10000:
    if env is None or env.terminated:
        env = make_env()
        obs = []
    env.step(random.choice(env.legal_action_list))
    obs.append(env.observation)
    frames += 1
tt = time.time() - _t
print(f"Time {tt}, Frames {frames}, FPS {frames / tt}")