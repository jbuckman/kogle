import random
import numpy as np
import cpp

class Kogle:
    game_name = None                # Override this to choose a game

    observation_shape = (64,64)     # The dimensions of a single observation
    frame_stack = 4                 # The dimensions of a single state
    state_tokens = 256              # Number of unique input values
    state_continuous = True         # Whether or not the input should be interpreted as continuous
    action_count = 5                # Maximum number of actions in any state

    discount = 1.                   # Discount to use for measuring the return
    random_termination_prob = 0.    # Chance of random mid-episode termination
    max_steps = None                # Number of steps before forced termination

    action_repeat = 4               # Number of times in a row to take an action
    sticky_action_prob = 1/4        # Chance that the environment repeats the last action instead of taking the new one

    def __init__(self, **overrides):
        for k, v in overrides.items(): setattr(self, k, v)
        self.kogle = getattr(cpp, self.game_name)()
        self.reset()

    @property
    def observation(self):
        return self.recent_obs_buffer[-1].copy()

    @property
    def state(self):
        return self.recent_obs_buffer.copy()

    @property
    def state_shape(self):
        return (self.frame_stack,) + self.observation_shape

    @property
    def legal_action_mask(self):
        return self.kogle.legalActions()

    @property
    def legal_action_set(self):
        return [item for item in range(self.action_count) if item in self.legal_action_mask]

    def reset(self):
        self.kogle.reset()
        self.steps = 0
        self.terminated = False
        self.stuck_action = None
        self.recent_obs_buffer = np.zeros(self.state_shape, dtype=np.uint8)
        self.kogle.renderPixels(self.recent_obs_buffer[-1])

    def step(self, action):
        reward = 0
        for i in range(self.action_repeat):
            if self.stuck_action is None or random.random() > self.sticky_action_prob: self.stuck_action = action
            _reward, self.terminated = self.kogle.step([i == self.stuck_action for i in range(5)])
            reward += _reward
            if self.terminated: break
        self.steps += 1
        self.terminated = self.terminated or \
                          (self.max_steps is not None and self.steps > self.max_steps) or \
                          (random.random() < self.random_termination_prob)
        if not self.terminated:
            self.recent_obs_buffer[:-1] = self.recent_obs_buffer[1:]
            self.kogle.renderPixels(self.recent_obs_buffer[-1])
        return reward
