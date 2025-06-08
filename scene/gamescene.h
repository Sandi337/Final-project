#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>

/*
   [game scene object]
*/
typedef enum EleType
{
    Character_L,
    Hud_L,
    Mushroom_L,
    Teleport_L,
    Tree_L,
    Projectile_L,
    Pause_button_L
} EleType;
typedef struct _GameScene
{
    ALLEGRO_BITMAP *background;
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    float bg_offset_x;          // 背景水平偏移量
    float scroll_speed;         // 滾動速度
    int width, height;
    double start_time;          //倒計時器

} GameScene;
Scene *New_GameScene(int label);
void game_scene_update(Scene *self, float delta_time);
void game_scene_draw(Scene *self);
void game_scene_destroy(Scene *self);

#endif
