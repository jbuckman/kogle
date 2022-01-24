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
- `env.reset()` restarts an episode. It is called automatically on initialization.
- `env.step(action)` takes a step in the environment and returns the reward.
- `env.observation` gets the current frame. `env.observation_shape` gives its shape.
- `env.state` gets the most recent `env.frame_stack` frames, concatenated along the first dimension. `env.state_shape` gives its shape.
- `env.legal_action_mask` gets a boolean mask of length `env.action_count` indicating which actions are legal in this state.
- `env.legal_action_set` gets an integer list of varying length corresponding to the legal actions.
- `env.terminated` gets a boolean corresponding to whether the environment has terminated.

Run `python benchmark_speed.py` to see it in action.