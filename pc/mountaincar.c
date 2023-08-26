#include <stdlib.h>
#include <math.h>
#include "mountaincar.h"

double MountainCarStep(MountainCarState *state, MountainCarAction action) {
    double position = state->position;
    double velocity = state->velocity;
    double force = action - 1;
    velocity += force * 0.001 + cos(3 * position) * (-0.0025);
    if (velocity > 0.07) {
        velocity = 0.07;
    } else if (velocity < -0.07) {
        velocity = -0.07;
    }
    position += velocity;
    if (position > 0.6) {
        position = 0.6;
        velocity = 0.0;
    } else if (position < -1.2) {
        position = -1.2;
        velocity = 0.0;
    }
    state->position = position;
    state->velocity = velocity;
    state->steps += 1;

    if (position >= 0.5 || state->steps >= MAX_EPISODE_LEN) {
        state->done = 1;
    }

    double reward = -1.0;
    // if (position > state->max_position) {
    //     reward += position - state->max_position;
    //     state->max_position = position;
    // }

    return reward;
}

void MountainCarReset(MountainCarState *state) {
    // uniform random between -0.6 and -0.4
    state->position = 0.2 * rand() / (double)RAND_MAX - 0.6;
    state->velocity = 0.0;
    state->max_position = state->position;
    state->steps = 0;
    state->done = 0;
}

