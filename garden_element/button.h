#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include "../element/element.h"  
#include "../element/element_label.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"
#include <stdbool.h>

/*
   [Button object]
*/
int final_score_global;
typedef enum ButtonType
{
    BUT_UP=0,
    BUT_DOWN,
    BUT_LEFT,
    BUT_RIGHT,
} ButtonType;
typedef struct _Button
{
    int x, y;
    int width, height;              // the width and height of image
    bool dir;
    bool state_changed;                       // 狀態更換確認
    int score;                          //分數
    ALLEGRO_FONT *font;
    double start_time;
    bool   finished;   
    double elapsed;
    double paused_time;         // 暫停累積時間
    double last_pause_start;    // 上次按下暫停的時間
    bool was_paused;            // 上次更新是否是 pause 狀態（偵測切換）
    ButtonType state;                      // the state of Button
    ALGIF_ANIMATION *gif_status[4]; // gif for each state. 0: stop, 1: move, 2:attack
    ALLEGRO_SAMPLE_INSTANCE *dance_Sound;
    int anime;      // counting the time of animation
    int anime_time; // indicate how long the animation
    bool new_proj;
    Shape *hitbox; // the hitbox of object
} Button;

Elements *New_Button(int label);
void Button_update(Elements *self, float delta_time);
void Button_interact(Elements *self);
void Button_draw(Elements *self);
void Button_destory(Elements *self);
void _Button_update_position(Elements *self, int dx, int dy);

#endif
