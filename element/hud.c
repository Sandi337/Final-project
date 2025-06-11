#include "hud.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "../element/charater.h"
#include <stdio.h>

#define MAX_HEALTH 100
#define MAX_ENERGY 100
#define MAX_SPIRIT 100

void Draw_HUD(Character *chara, ALLEGRO_FONT *font, int screen_width) {
    if (!chara || !font) return;

    int bar_width = 230;
    int bar_height = 25;
    int base_x = screen_width - 240;
    int base_y = 20;

    float hp_ratio = (float)chara->status->HP / MAX_HEALTH;
    float en_ratio = (float)chara->status->EN / MAX_ENERGY;
    float sp_ratio = (float)chara->status->SP / MAX_SPIRIT;

    // HP bar
    al_draw_filled_rectangle(base_x, base_y, base_x + bar_width, base_y + bar_height, al_map_rgb(100, 0, 0));
    al_draw_filled_rectangle(base_x, base_y, base_x + (bar_width * hp_ratio), base_y + bar_height, al_map_rgb(120, 0, 0));
    al_draw_textf(font, al_map_rgb(255, 255, 255), base_x + 5, base_y, 0, "HEALTH: %d / %d", chara->status->HP, MAX_HEALTH);

    // Energy bar
    base_y += 30;
    al_draw_filled_rectangle(base_x, base_y, base_x + bar_width, base_y + bar_height, al_map_rgb(100, 100, 0));
    al_draw_filled_rectangle(base_x, base_y, base_x + (bar_width * en_ratio), base_y + bar_height, al_map_rgb(233, 196, 106));
    al_draw_textf(font, al_map_rgb(255, 255, 255), base_x + 5, base_y, 0, "Energy: %d / %d", chara->status->EN, MAX_ENERGY);

    // Spirit bar
    base_y += 30;
    al_draw_filled_rectangle(base_x, base_y, base_x + bar_width, base_y + bar_height, al_map_rgb(0, 50, 100));
    al_draw_filled_rectangle(base_x, base_y, base_x + (bar_width * sp_ratio), base_y + bar_height, al_map_rgb(102, 155, 188));
    al_draw_textf(font, al_map_rgb(255, 255, 255), base_x + 5, base_y, 0, "Spirit: %d / %d", chara->status->SP, MAX_SPIRIT);
}
