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
// .cをincludeする。sizeof()を使いたいため。
#include "bg.c"
#include "bg_map_offseted.c"

void train_main() {
    QLearningState *q_state = QLearningStateCreate();
    for (int epoch = 0; epoch < 1000; epoch++) {
        initrand((unsigned int)epoch);
        for (int i = 0; i < 100; i++) {
            TrainEpisode(q_state);
        }
        // print_table(q_state);
        // fix test case
        initrand((unsigned int)1);
        int32_t total_reward = 0;
        for (int i = 0; i < 10; i++) {
            total_reward += TestEpisode(q_state);
        }
        gotoxy(0, 16);
        printf("%d,Avg:%d", epoch, total_reward / 10);
    }
}

int main() {
    DISPLAY_OFF;
    SHOW_BKG;SHOW_SPRITES;
    // 初回のputs/printfで、フォントデータ（背景データ）の設定、タイルの初期化が行われる。そのあとで独自のタイルを設定する。
    gotoxy(0, 15);
    putchar('0');
    set_bkg_data(128, sizeof(TileData) / 16, TileData);
    set_bkg_tiles(0, 0, 20, 18, MapData);
    gotoxy(0, 16);
    puts("Training...");
    DISPLAY_ON;
    train_main();
    return 0;
}
