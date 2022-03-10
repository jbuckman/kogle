# King of Games Learning Environment

Requirements:
```
pip install pybind11
apt-get install clang llvm
```
Build instructions:
```
chmod u+x i
./i
```
To use:
```
from kogle.kogle import Kogle
env = Kogle(game_name="Breakout", sticky_action_prob=0.1)
```
The interface is pretty self-explanatory.
- `env.step(action)` takes a step in the environment and returns the reward.
- `env.observation` gets the current frame. `env.observation_shape` gives its shape.
- `env.state` gets the most recent `env.obs_stack` frames, concatenated along the first dimension. `env.state_shape` gives its shape.
- `env.legal_action_mask` gets a boolean mask of length `env.action_count` indicating which actions are legal in this state.
- `env.legal_action_set` gets an integer set containing the legal actions.
- `env.terminated` gets a boolean corresponding to whether the environment has terminated.

Run `python interactive.py` to get an interactive version of the game that can be played with arrow keys. (Requires `tkinter`.) Use spacebar to fire, z for no-op, and WASD to move and fire at the same time.

Run `python benchmark_speed.py` to measure the FPS.