#include <stdio.h>
#include <stdlib.h>
#include <gb/gb.h>
#include <gbdk/console.h>

#include "config.h"
#include "mountaincar.h"
#include "qlearning.h"
#include "trained.h"
#include "display.h"

#define STEP_CYCLE 2
typedef struct
{
    MountainCarState state;
    uint8_t cycle;
} RunPretrainedState;

// 学習済みモデル実行用の状態
static RunPretrainedState *rps;
// 同時に実行されないモード間でunionによりメモリ領域を静的に共有したほうがポインタアクセスが簡単になりいいかもしれない。

void run_pretrained_vbl()
{
    const QLearningState *q_state = (const QLearningState *)(pc_trained_params);

    if (--rps->cycle == 0)
    {
        rps->cycle = 2;
        step_display(q_state, &rps->state);
    }
}

void run_pretrained_main()
{
    gotoxy(0, 16);
    printf("Running pre-trained model");
    rps = malloc(sizeof(RunPretrainedState));
    MountainCarReset(&rps->state);
    rps->cycle = STEP_CYCLE;
    __critical
    {
        add_VBL(run_pretrained_vbl);
    }

    while (1)
    {
        wait_vbl_done();
    }
}
