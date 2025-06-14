#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"
#include "../element/element_label.h"  
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>

/*
   [game scene object]
*/

typedef struct _GameScene
{
    ALLEGRO_BITMAP *background;
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    CharacterStatus *status;
    float bg_offset_x;          // 背景水平偏移量
    float scroll_speed;         // 滾動速度
    int width, height;
    double start_time;          //倒計時器
    float energy_timer;         // 每 3 秒扣能量
    bool garden_portal_spawned; //入口是否生成過
    double garden_portal_timer;
    bool has_switched_to_sea;   //是否切換到海洋
    

} GameScene;
Scene *New_GameScene(int label, CharacterStatus *status);
Scene *New_GardenScene(int label, CharacterStatus *status);
void game_scene_update(Scene *self, float delta_time);
void game_scene_draw(Scene *self);
void game_scene_destroy(Scene *self);

#endif
