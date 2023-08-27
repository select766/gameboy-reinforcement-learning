#include <stdio.h>
#include <stdlib.h>
#include <gb/gb.h>
#include <gbdk/console.h>

#include <gbdk/platform.h>
#include <gbdk/font.h>
#include <gbdk/bcd.h>

#include "config.h"
#include "mountaincar.h"
#include "qlearning.h"
#include "train.h"
#include "display.h"

#define STEP_CYCLE 2

typedef struct
{
    QLearningState *q_state;
    MountainCarState state;
    uint8_t cycle;
    uint8_t updated;
    int epoch;
    uint8_t avg_steps;
} TrainState;

static TrainState *ts;

// VBL割り込み関数側では、現在のモデルを用いて独立にエピソードを実行し、車の動きを可視化する。
void train_vbl()
{
    const QLearningState *q_state = ts->q_state;

    if (--ts->cycle == 0)
    {
        ts->cycle = STEP_CYCLE;
        step_display(q_state, &ts->state);
    }

    if (ts->updated)
    {
        ts->updated = 0;
        gotoxy(0, 16);
        printf("%d00 episodes\nAvg steps: %d", ts->epoch, ts->avg_steps);
    }
}

void train_main()
{
    ts = malloc(sizeof(TrainState));
    MountainCarReset(&ts->state);
    QLearningState *q_state = QLearningStateCreate();
    ts->q_state = q_state;
    ts->cycle = STEP_CYCLE;
    ts->updated = 1;
    ts->epoch = 0;
    ts->avg_steps = MAX_EPISODE_LEN;

    __critical
    {
        add_VBL(train_vbl);
    }

    // main関数側では、割り込み処理をしていない間ずっとモデルの学習を行う。
    for (int epoch = 0; epoch < 1000; epoch++)
    {
        initrand((unsigned int)epoch);
        for (int i = 0; i < 100; i++)
        {
            TrainEpisode(q_state);
        }
        // print_table(q_state);
        // fix test case
        initrand((unsigned int)1);
        uint32_t total_steps = 0;
        for (int i = 0; i < 10; i++)
        {
            uint8_t steps;
            TestEpisode(q_state, &steps);
            total_steps += steps;
        }

        __critical
        {
            ts->epoch = epoch + 1;
            ts->avg_steps = total_steps / 10;
            ts->updated = 1;
        }
    }
}
