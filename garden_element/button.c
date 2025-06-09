#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include "../element/element_label.h"
#include "button.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"
#include "../scene/gardenscene.h"
#include "../scene/gamefinish.h"
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include <stdbool.h>
#define MAX_TIME 20.0
/*
   [Button function]
*/

Elements *New_Button(int label)
{
    srand(time(NULL));
    al_init_font_addon();
    al_init_ttf_addon();
    Button *pDerivedObj = (Button *)malloc(sizeof(Button));
    Elements *pObj = New_Elements(Button_L);
    // setting derived object member
    // load Button images
    char state_string[4][10] = {"up", "down", "left", "right"};
    for (int i = 0; i < 4; i++)
    {
        char buffer[50];
        sprintf(buffer, "assets/image/mushrooms_%s.gif", state_string[i]);
        pDerivedObj->gif_status[i] = algif_new_gif(buffer, -1);
    }
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/dance_button1.mp3");
    pDerivedObj->dance_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->dance_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->dance_Sound, al_get_default_mixer());
    // initial the geometric information of Button
    pDerivedObj->width = pDerivedObj->gif_status[0]->width;
    pDerivedObj->height = pDerivedObj->gif_status[0]->height;
    pDerivedObj->x = 405;
    pDerivedObj->y = HEIGHT - pDerivedObj->height - 275;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
    pDerivedObj->state_changed = false; // true: 可變更, false: 不可變更
    pDerivedObj->score=0;
    pDerivedObj->start_time = al_get_time(); 
    pDerivedObj->finished   = false;
    pDerivedObj->font=al_load_ttf_font("assets/font/pirulen.ttf",50,0);
    pDerivedObj->paused_time = 0.0;
    pDerivedObj->last_pause_start = 0.0;
    pDerivedObj->was_paused = false;
    pDerivedObj->elapsed= 0.0;
    
    // initial the animation component
    pDerivedObj->state = BUT_LEFT;
    pDerivedObj->new_proj = false;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Button_draw;
    pObj->Update = Button_update;
    pObj->Interact = Button_interact;
    pObj->Destroy = Button_destory;
    return pObj;
}

void Button_update(Elements *self, float delta_time)
{
    // use the idea of finite state machine to deal with different state
    Button *chara = ((Button *)(self->pDerivedObj));
    double now = al_get_time();
    if (is_paused && !chara->was_paused) {
        // 剛剛被暫停
        chara->last_pause_start = now;
        chara->was_paused = true;
    } else if (!is_paused && chara->was_paused) {
        // 剛剛從暫停恢復
        chara->paused_time += now - chara->last_pause_start;
        chara->was_paused = false;
    }

    if(!is_paused){
        chara->elapsed =now - chara->paused_time-chara->start_time;
        if (chara->elapsed >= MAX_TIME) {
            final_score_global = chara->score;
            chara->finished = true;
            return;
        }

        if(chara->state_changed == true){
            chara->score+=10;
            int new_state=rand()%4;
            chara->state = new_state;
            chara->state_changed = false;
        }
        if (chara->state == BUT_UP &&key_state[ALLEGRO_KEY_UP])
            chara->state_changed = true;
        else if (chara->state == BUT_DOWN&&key_state[ALLEGRO_KEY_DOWN])
            chara->state_changed = true;
        else if (chara->state == BUT_LEFT&&key_state[ALLEGRO_KEY_LEFT])
            chara->state_changed = true;
        else if (chara->state == BUT_RIGHT&&key_state[ALLEGRO_KEY_RIGHT])
            chara->state_changed = true;
    }
    if(legend){
        chara->score=1000;
    }
}

void Button_draw(Elements *self)
{
    // with the state, draw corresponding image
    Button *chara = ((Button *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(chara->gif_status[chara->state], al_get_time());
    if (is_paused) {
    al_draw_text(chara->font, al_map_rgb(255, 0, 0), WIDTH / 2, 20, ALLEGRO_ALIGN_CENTER, "PAUSED");
    }

    int draw_x=chara->x;

    if (chara->state == BUT_UP)
            draw_x = WIDTH/2-64-100;
    else if (chara->state == BUT_DOWN)
            draw_x = WIDTH/2-64+100;
    else if (chara->state == BUT_LEFT)
            draw_x = WIDTH/2-64-300;
    else if (chara->state == BUT_RIGHT)
            draw_x = WIDTH/2-64+300;

    if (frame)
    {
        al_draw_bitmap(frame, draw_x, chara->y, 0);
    }

    char score_text[50];
    sprintf(score_text, "Score: %d", chara->score);
    al_draw_text(chara->font, al_map_rgb(255, 255, 255), 20, 20, 0, score_text);
    double remain = MAX_TIME - chara->elapsed;
    if (remain < 0) remain = 0; 
    char time_text[32];
    sprintf(time_text, "Time: %.0f s", remain);
    al_draw_text(chara->font, al_map_rgb(255,255,0), 20, 70, 0, time_text);
    if ((chara->state == BUT_UP &&key_state[ALLEGRO_KEY_UP])||(chara->state == BUT_DOWN&&key_state[ALLEGRO_KEY_DOWN])||(chara->state == BUT_LEFT&&key_state[ALLEGRO_KEY_LEFT])||(chara->state == BUT_RIGHT&&key_state[ALLEGRO_KEY_RIGHT]))
    {
        al_play_sample_instance(chara->dance_Sound);
    }
}
void Button_destory(Elements *self)
{
    Button *Obj = ((Button *)(self->pDerivedObj));
    for (int i = 0; i < 4; i++)
        algif_destroy_animation(Obj->gif_status[i]);
    al_destroy_font(Obj->font);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}

void _Button_update_position(Elements *self, int dx, int dy)
{
    Button *chara = ((Button *)(self->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

void Button_interact(Elements *self) {}
