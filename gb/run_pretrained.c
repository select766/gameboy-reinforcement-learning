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
#include "trained.h"

const uint8_t display_y_table[160] = {54, 55, 57, 59, 60, 62, 64, 66, 67, 69, 71, 73, 75,
                                   76, 78, 80, 82, 84, 86, 87, 89, 91, 93, 94, 96, 98,
                                   99, 101, 103, 104, 106, 107, 109, 110, 112, 113, 115, 116, 117,
                                   118, 120, 121, 122, 123, 124, 125, 125, 126, 127, 128, 128, 129,
                                   129, 130, 130, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
                                   131, 130, 130, 130, 129, 129, 128, 127, 126, 126, 125, 124, 123,
                                   122, 121, 120, 119, 117, 116, 115, 114, 112, 111, 109, 108, 106,
                                   105, 103, 102, 100, 98, 97, 95, 93, 91, 90, 88, 86, 84,
                                   82, 81, 79, 77, 75, 73, 71, 70, 68, 66, 64, 63, 61,
                                   59, 57, 56, 54, 52, 51, 49, 48, 46, 45, 43, 42, 41,
                                   39, 38, 37, 36, 34, 33, 32, 31, 30, 30, 29, 28, 27,
                                   27, 26, 26, 25, 25, 24, 24, 24, 24, 24, 24, 24, 24,
                                   24, 24, 24, 25};

#define STEP_CYCLE 2
typedef struct {
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
        // -12000 to 6000
        // 0 to 159
        uint8_t center_x = (uint8_t)((uint16_t)(rps->state.position + 12000) / (18000 / 160));
        // move_spriteの引数xは、x=8でスプライトの左端が画面の左端になる。x=center_xのときに、スプライト中心が画面の左端に来てほしい。スプライトの幅は8。
        move_sprite(0, center_x + 4, display_y_table[center_x]);
        MountainCarAction action = GetBestAction(q_state, &rps->state);
        Reward reward = MountainCarStep(&rps->state, action);
        if (rps->state.done)
        {
            MountainCarReset(&rps->state);
        }
    }
}

void run_pretrained_main()
{
    gotoxy(0, 16);
    printf("Running pre-trained model");
    rps = malloc(sizeof(RunPretrainedState));
    MountainCarReset(&rps->state);
    rps->cycle = STEP_CYCLE;
    __critical {
        add_VBL(run_pretrained_vbl);
    }

    while (1)
    {
        wait_vbl_done();
    }
}
