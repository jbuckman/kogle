import random, time, sys

from ale_py import ALEInterface, LoggerMode
ALEInterface.setLoggerMode(LoggerMode.Warning)
import ale_py.roms

ale = ALEInterface()
ale.loadROM(getattr(ale_py.roms, sys.argv[1]))

termination = True
_t = time.time()
obs = []
frames = 0
while frames < 10000:
    if termination:
        ale.reset_game()
        obs = []
    ale.act(random.choice(ale.getLegalActionSet()))
    obs.append(ale.getScreenGrayscale())
    termination = ale.game_over()
    frames += 1
tt = time.time() - _t
print(f"Time {tt}, Frames {frames}, FPS {frames / tt}")