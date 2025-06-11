#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "charater.h"
#include "projectile.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"
#include "../scene/gamescene.h"
#include <stdio.h>
#include <stdbool.h>

#define MAX_walk_area 1.7
#define DEBUG_HITBOX 0
/*
   [Character function]
*/
Elements *New_Character(int label, CharacterStatus *status)
{
    Character *pDerivedObj = (Character *)malloc(sizeof(Character));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load character images
    char state_string[3][10] = {"stop", "move", "attack"};
    for (int i = 0; i < 3; i++)
    {
        char buffer[50];
        sprintf(buffer, "assets/image/ichigo_%s.gif", state_string[i]);
        pDerivedObj->gif_status[i] = algif_new_gif(buffer, -1);
    }

    // initial the geometric information of character
    pDerivedObj->width = pDerivedObj->gif_status[0]->width;
    pDerivedObj->height = pDerivedObj->gif_status[0]->height;
    pDerivedObj->x = 300;
    pDerivedObj->y = HEIGHT - pDerivedObj->height - 60;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y,
                                        pDerivedObj->x  + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y + pDerivedObj->height);
    pDerivedObj->dir = false; // true: face to right, false: face to left
    // initial the animation component
    pDerivedObj->state = STOP;
    pDerivedObj->new_proj = false;
    pDerivedObj->status = status;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Character_draw;
    pObj->Update = Character_update;
    pObj->Interact = Character_interact;
    pObj->Destroy = Character_destory;
    return pObj;
}
void Character_update(Elements *self, float delta_time)
{
    // use the idea of finite state machine to deal with different state
    Character *chara = ((Character *)(self->pDerivedObj));
    Rectangle *rect = (Rectangle *)(chara->hitbox->pDerivedObj);
    rect->x1 = chara->x + chara->width / 3;
    rect->y1 = chara->y ;
    rect->x2 = chara->x + 2 * chara->width / 3;
    rect->y2 = chara->y + chara->height; 

    if (chara->state == STOP)
    {
        if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            chara->state = MOVE;
        }
        else if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
            chara->state = MOVE;
        }
        else if (key_state[ALLEGRO_KEY_W] || key_state[ALLEGRO_KEY_S])
        {
            chara->state = MOVE;
        }
        else
        {
            chara->state = STOP;
        }
    }
    else if (chara->state == MOVE)
    {
        if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            if (chara->x > 0) {
                _Character_update_position(self, -5, 0);  // 不讓角色走出畫面
            }
            chara->state = MOVE;
        }
        else if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
             // 判斷是否角色還在畫面左半邊 → 允許移動；否則停下讓背景捲動
            if (chara->x < WIDTH / 3.0f) {
                _Character_update_position(self, +5, 0);
            }
            // 否則不要更新 x，背景會在 game_scene_update() 裡移動
            chara->state = MOVE;
        }
        else if (key_state[ALLEGRO_KEY_W])
        {
            _Character_update_position(self, 0, -5);
            chara->state = MOVE;
        }
        else if (key_state[ALLEGRO_KEY_S])
        {
            _Character_update_position(self, 0, 5);
            chara->state = MOVE;
        }
              
        if (chara->gif_status[chara->state]->done)
            chara->state = STOP;
    }
    else if (chara->state == ATK)
    {
        if (chara->gif_status[ATK]->display_index == 2 && chara->new_proj == false)
        {
            Elements *pro;
            if (chara->dir)
            {
                pro = New_Projectile(Projectile_L,
                                     chara->x + chara->width - 100,
                                     chara->y + 10,
                                     0);
            }
            else
            {
                pro = New_Projectile(Projectile_L,
                                     chara->x - 50,
                                     chara->y + 10,
                                     0);
            }
            _Register_elements(scene, pro);
            chara->new_proj = true;
            
            //al_play_sample_instance(chara->atk_Sound);
        }
        if (chara->gif_status[chara->state]->done)
        {
            chara->state = STOP;
            chara->new_proj = false;
        }
    }
}
void Character_draw(Elements *self)
{
    // with the state, draw corresponding image
    Character *chara = ((Character *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(chara->gif_status[chara->state], al_get_time());
    if (frame)
    {
        al_draw_bitmap(frame, chara->x, chara->y, ((chara->dir) ? ALLEGRO_FLIP_HORIZONTAL : 0));
    }

    #if DEBUG_HITBOX
    if (chara->hitbox && chara->hitbox->pDerivedObj && chara->hitbox->getType && chara->hitbox->getType() == RECTANGLE) {
        Rectangle *rect = (Rectangle *)(chara->hitbox->pDerivedObj);
        al_draw_rectangle(
            rect->x1, rect->y1,
            rect->x2, rect->y2,
            al_map_rgb(255, 0, 0),2
        );
    }
    #endif
}
void Character_destory(Elements *self)
{
    Character *Obj = ((Character *)(self->pDerivedObj));
    //al_destroy_sample_instance(Obj->atk_Sound);
    for (int i = 0; i < 3; i++)
        algif_destroy_animation(Obj->gif_status[i]);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}

void _Character_update_position(Elements *self, int dx, int dy)
{
    Character *chara = ((Character *)(self->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
    if(chara-> y < HEIGHT/MAX_walk_area)  chara ->y = HEIGHT/MAX_walk_area; //上界是畫面的1.7倍高
    if(chara-> y > HEIGHT- chara ->height) chara ->y = HEIGHT- chara ->height;//下界不超過底部
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox,  chara->x + chara->width / 2);
    hitbox->update_center_y(hitbox,  chara->y + chara->height / 2);
}

void Character_interact(Elements *self) {}

