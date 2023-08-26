#include <stdlib.h>
#include "config.h"
#include "mountaincar.h"

#define POSITION_SCALE 10000
#define FORCE 10
#define VELOCITY_MAX 700
#define VELOCITY_MIN -700
#define POSITION_MIN -12000
#define POSITION_MAX 6000
#define POSITION_DONE 5000

/*
raw_values = np.arange(512//2,18000+512,512)
(np.cos(3*(raw_values / scale - 1.2))*-0.0025*10000).astype(np.int8)
*/
static const int8_t gravity_table[] = {
    23,  24,  24,  24,  24,  23,  21,  19,  16,  13,  10,   6,   2,
        -1,  -4,  -8, -12, -15, -18, -20, -22, -23, -24, -24, -24, -23,
       -22, -20, -17, -14, -11,  -8,  -4,   0,   3, 6
};

static Velocity gravity(Position position) {
    // cos(3 * position) * (-0.0025);
    return gravity_table[(position - POSITION_MIN) >> 9];
}

Reward MountainCarStep(MountainCarState *state, MountainCarAction action) {
    Position position = state->position;
    Velocity velocity = state->velocity;
    switch (action) {
        case 0:
            velocity -= FORCE;
            break;
        case 2:
            velocity += FORCE;
            break;
    }
    velocity += gravity(position);
    if (velocity > VELOCITY_MAX) {
        velocity = VELOCITY_MAX;
    } else if (velocity < VELOCITY_MIN) {
        velocity = VELOCITY_MIN;
    }
    position += velocity;
    if (position > POSITION_MAX) {
        position = POSITION_MAX;
        velocity = 0;
    } else if (position < POSITION_MIN) {
        position = POSITION_MIN;
        velocity = 0;
    }
    state->position = position;
    state->velocity = velocity;
    state->steps += 1;


    Reward reward = REWARD_PER_STEP;

    if (position >= POSITION_DONE) {
        reward += REWARD_AT_GOAL;
    }

    if (position >= POSITION_DONE || state->steps >= MAX_EPISODE_LEN) {
        state->done = 1;
    }

    return reward;
}

void MountainCarReset(MountainCarState *state) {
    // uniform random between -0.6 and -0.4
    // (0.2 * rand() / RAND_MAX - 0.6) * 10000;
    
#ifdef __SDCC
    // randw(): 0 to 0xffff
    // randw()>>5: 0 to 2047
    state->position = (int16_t)(randw() >> 5) - 6000;
#else
    // rand(): 0 to 0x7fff
    // rand()>>4: 0 to 2047
    state->position = (rand() >> 4) - 6000;
#endif
    state->velocity = 0;
    state->steps = 0;
    state->done = 0;
}

