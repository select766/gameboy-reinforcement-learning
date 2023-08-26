// PC上で学習を試みる。
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "mountaincar.h"
#include "qlearning.h"

void print_table(QLearningState *q_state) {
    for (int i = 0; i < N_STATE_0 * N_STATE_1; i++) {
        if (i % N_STATE_1 == 0 && i != 0) {
            printf("\n");
        }
        Reward max_q = q_state->q_table[i][0];
        for (int j = 0; j < N_ACTIONS; j++) {
            if (q_state->q_table[i][j] > max_q) {
                max_q = q_state->q_table[i][j];
            }
        }
        printf("%04d ", max_q);
    }
    printf("\n");
}

int main() {
    QLearningState *q_state = QLearningStateCreate();
    for (int epoch = 0; epoch < 1000; epoch++) {
        srand((unsigned int)epoch);
        for (int i = 0; i < 100; i++) {
            TrainEpisode(q_state);
        }
        // print_table(q_state);
        // fix test case
        srand((unsigned int)1);
        int32_t total_reward = 0;
        for (int i = 0; i < 10; i++) {
            total_reward += TestEpisode(q_state);
        }
        printf("%d,Avg:%d\n", epoch, total_reward / 10);
    }

    FILE* result_file = fopen("trained.bin", "wb");
    fwrite(q_state, sizeof(QLearningState), 1, result_file);
    fclose(result_file);
    return 0;
}
