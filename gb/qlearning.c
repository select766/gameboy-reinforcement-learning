#include <stdlib.h>
#include "config.h"
#include "mountaincar.h"
#include "qlearning.h"

QLearningState *QLearningStateCreate() {
    QLearningState *state = malloc(sizeof(QLearningState));
    for (int i = 0; i < N_STATE_0 * N_STATE_1; i++) {
        for (int j = 0; j < N_ACTIONS; j++) {
            state->q_table[i][j] = 0;
        }
    }
    return state;
}

// (np.linspace(-1.2,0.6,16+1)*10000)[1:-1].astype(np.int32)
static const Position position_thresholds[] = {-10875,  -9750,  -8625,  -7500,  -6375,  -5250,  -4125,  -3000,
        -1875,   -749,    374,   1499,   2624,   3749,   4874};
static const Velocity velocity_thresholds[] = {-612, -525, -437, -350, -262, -175,  -87,    0,   87,  175,  262,
        350,  437,  525,  612};

static int get_state_index(MountainCarState* state)
{
    // int position = (int)((state->position / 10000.0 + 1.2) / 1.8 * N_STATE_0);
    // int velocity = (int)((state->velocity / 10000.0 + 0.07) / 0.14 * N_STATE_1);
    uint8_t position;
    for (position = 0; position < N_STATE_0 - 1; position++) {
        if (state->position < position_thresholds[position]) {
            break;
        }
    }
    uint8_t velocity;
    for (velocity = 0; velocity < N_STATE_1 - 1; velocity++) {
        if (state->velocity < velocity_thresholds[velocity]) {
            break;
        }
    }
    return position * N_STATE_1 + velocity;
}

Reward TestEpisode(QLearningState *q_state) {
    MountainCarState state;
    MountainCarReset(&state);
    Reward total_reward = 0;
    while (!state.done) {
        // choose action
        MountainCarAction action = 0;
        int state_index = get_state_index(&state);
        Reward max_q = q_state->q_table[state_index][0];
        for (uint8_t i = 1; i < N_ACTIONS; i++) {
            if (q_state->q_table[state_index][i] > max_q) {
                max_q = q_state->q_table[state_index][i];
                action = i;
            }
        }
        Reward reward = MountainCarStep(&state, action);
        total_reward += reward;
    }
    return total_reward;
}

Reward TrainEpisode(QLearningState *q_state) {
    MountainCarState state;
    MountainCarReset(&state);
    Reward total_reward = 0;
    while (!state.done) {
        // choose action
        MountainCarAction action = 0;
        int state_index = get_state_index(&state);
        if (rand() < EPSILON_RAND_MAX) {
            // random action
            // TODO: 3で割るの重いかも
            MountainCarAction action = rand() % N_ACTIONS;
        } else {
            Reward max_q = q_state->q_table[state_index][0];
            for (uint8_t i = 1; i < N_ACTIONS; i++) {
                if (q_state->q_table[state_index][i] > max_q) {
                    max_q = q_state->q_table[state_index][i];
                    action = i;
                }
            }
        }
        Reward reward = MountainCarStep(&state, action);
        total_reward += reward;

        // update model
        Reward max_q_next = 0;
        if (!state.done) {
            int next_state_index = get_state_index(&state);
            max_q_next = q_state->q_table[next_state_index][0];
            for (uint8_t i = 1; i < N_ACTIONS; i++) {
                if (q_state->q_table[next_state_index][i] > max_q_next) {
                    max_q_next = q_state->q_table[next_state_index][i];
                }
            }
        }

        Reward q_value = q_state->q_table[state_index][action];
        q_state->q_table[state_index][action] = ((reward + max_q_next - (max_q_next >> GAMMA_BITSHIFT) - q_value) >> ALPHA_BITSHIFT) + q_value;
        // for Reward is double
        // q_state->q_table[state_index][action] = (reward + max_q_next * 0.984375 - q_value) * 0.015625 + q_value;
    }

    return total_reward;
}
