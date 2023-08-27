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
#include "run_pretrained.h"
// .cをincludeする。sizeof()を使いたいため。
#include "bg.c"
#include "bg_map_offseted.c"
#include "sprite.c"

void train_main()
{
    QLearningState *q_state = QLearningStateCreate();
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
        int32_t total_reward = 0;
        for (int i = 0; i < 10; i++)
        {
            total_reward += TestEpisode(q_state);
        }
        gotoxy(0, 16);
        printf("%d,Avg:%d", epoch, total_reward / 10);
    }
}

uint8_t choice_mode()
{
    printf("GB MOUNTAIN CAR\nREINFORCEMENT LEARNING\n\nA: Train\nB: Run pre-trained\n\n\nby @select766");
    uint8_t mode = 0;
    while (!mode)
    {
        uint8_t button = joypad();
        if (button & J_A)
        {
            mode = 1;
        }
        else if (button & J_B)
        {
            mode = 2;
        }
        wait_vbl_done();
    }

    return mode;
}

int main()
{
    uint8_t mode = choice_mode();

    DISPLAY_OFF;
    SHOW_BKG;
    SHOW_SPRITES;
    // 初回のputs/printfで、フォントデータ（背景データ）の設定、タイルの初期化が行われる。フォントデータと干渉しない場所で独自のタイルを設定する。文字が出力された座標だけでなく画面全体のタイルが初期化されることに注意。
    set_bkg_data(128, sizeof(TileData) / 16, TileData);
    set_bkg_tiles(0, 0, 20, 18, MapData);

    set_sprite_data(0, sizeof(SpriteLabel) / 16, SpriteLabel);
    set_sprite_tile(0, 0);
    DISPLAY_ON;

    switch (mode) {
        case 1:
            train_main();
            break;
        case 2:
            run_pretrained_main();
            break;
    }

    return 0;
}
