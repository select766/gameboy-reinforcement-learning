#pragma once
#define N_STATE_0 20
#define N_STATE_1 20
#define N_ACTIONS 3

typedef struct {
    double q_table[N_STATE_0 * N_STATE_1][N_ACTIONS];
    double gamma, alpha, epsilon;
} QLearningState;

QLearningState *QLearningStateCreate();
double TestEpisode(QLearningState *q_state);
double TrainEpisode(QLearningState *q_state);
