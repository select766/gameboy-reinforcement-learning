#pragma once
#include "mountaincar.h"

typedef struct {
    Reward q_table[N_STATE_0 * N_STATE_1][N_ACTIONS];
} QLearningState;

QLearningState *QLearningStateCreate();
Reward TestEpisode(QLearningState *q_state);
Reward TrainEpisode(QLearningState *q_state);
