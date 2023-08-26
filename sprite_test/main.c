/*
スプライトを出してみるテスト
*/

#include <stdio.h>
#include <stdlib.h>
#include <gb/gb.h>
#include "tile.h"
#include "map.h"

int main() {
    DISPLAY_OFF;
    SHOW_BKG;SHOW_SPRITES;
    set_bkg_data(0, 4, TileLabel);
    set_bkg_tiles(0, 0, 10, 10, map1);
    set_sprite_data(0, 1, TileLabel + 16 * 4);
    set_sprite_tile(0, 0);
    move_sprite(0,8,30);
    DISPLAY_ON;
    while (1)
    {
        wait_vbl_done();
    }
}
