#pragma once
#include "mountaincar.h"

typedef struct {
    Reward q_table[N_STATE_0 * N_STATE_1][N_ACTIONS];
} QLearningState;

QLearningState *QLearningStateCreate();
MountainCarAction GetBestAction(const QLearningState *q_state, const MountainCarState* state);
Reward TestEpisode(const QLearningState *q_state, uint8_t *steps);
Reward TrainEpisode(QLearningState *q_state);
