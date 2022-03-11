import random
import numpy as np
import cpp as games

ACTIONS = NOOP, FIRE, UP, DOWN, LEFT, RIGHT, UPFIRE, DOWNFIRE, LEFTFIRE, RIGHTFIRE = list(range(10))

class Kogle:
    game_name = None                # Override this to choose a game

    observation_shape = (64,64)     # The dimensions of a single observation
    obs_stack = 4                   # The number of observations to stack into a state
    state_tokens = 256              # Number of unique input values
    state_continuous = True         # Whether or not the input should be interpreted as continuous
    action_count = len(ACTIONS)     # Maximum number of actions in any state

    discount = 1.                   # Discount to use for measuring the return
    random_termination_prob = 0.    # Chance of random mid-episode termination
    max_steps = None                # Number of steps before forced termination

    action_repeat = 4               # Number of times in a row to take an action
    sticky_action_prob = 1/4        # Chance that the environment repeats the last action instead of taking the new one

    def __init__(self, **overrides):
        for k, v in overrides.items(): setattr(self, k, v)
        self.kogle = getattr(games, self.game_name)()
        self.steps = 0
        self.terminated = False
        self.stuck_action = None
        self.recent_obs_buffer = np.zeros(self.state_shape, dtype=np.uint8)
        self.kogle.renderPixels(self.recent_obs_buffer[-1])

    @property
    def legal_action_mask(self):
        return self.kogle.legalActions()

    @property
    def legal_action_list(self):
        return [action for action in ACTIONS if self.is_legal_action(action)]

    @property
    def observation(self):
        return self.recent_obs_buffer[-1].copy()

    @property
    def state(self):
        return self.recent_obs_buffer.copy()

    @property
    def state_shape(self):
        return (self.obs_stack,) + self.observation_shape

    def is_legal_action(self, action):
        return self.legal_action_mask[action]

    def step(self, action):
        assert self.is_legal_action(action)
        reward = 0
        for i in range(self.action_repeat):
            if self.stuck_action is None or random.random() > self.sticky_action_prob: self.stuck_action = action
            _reward, self.terminated = self.kogle.step(self.stuck_action)
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
