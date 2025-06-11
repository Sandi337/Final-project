#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "charater_sea.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"
#include "../scene/seascene.h"
#include <stdio.h>
#include <stdbool.h>
// # define DEG_TO_RAD(x) ((x) * ALLEGRO_PI / 180.0)
/*
   [Character function]
*/
Elements *New_Character_SEA(int label, CharacterStatus *status)
{
    Character_SEA *pDerivedObj = (Character_SEA *)malloc(sizeof(Character_SEA));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load character images
    pDerivedObj->gif_Ichi = algif_new_gif("assets/image/swimming.gif", -1);

    // initial the geometric information of character
    pDerivedObj->x = WIDTH/2;
    pDerivedObj->y = HEIGHT/2 + 200;
    /*-------------*/
    pDerivedObj->angle = 0;
    pDerivedObj->balance_speed = 1.5;
    pDerivedObj->random_force = 0;
    pDerivedObj->last_force_angle_time = al_get_time(); // 上次改隨機力的時間
    pDerivedObj->force_angle_time_interval = 1;
    
    // 精神值
    pDerivedObj->spirit = 100;
    /*-------------*/
    pDerivedObj->new_proj = false;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Character_SEA_draw;
    pObj->Update = Character_SEA_update;
    pObj->Interact = Character_SEA_interact;
    pObj->Destroy = Character_SEA_destory;
    return pObj;
}
void Character_SEA_update(Elements *self, float delta_time)
{
    // use the idea of finite state machine to deal with different state
    Character_SEA *chara = ((Character_SEA *)(self->pDerivedObj));
    //
    double now = al_get_time();
    if ((now - chara->last_force_angle_time) > chara->force_angle_time_interval)
    {
        chara->random_force = ((rand() % 200) - 100) / 100.0; // -1 ~ +1
        chara->last_force_angle_time = now;
        chara->angle += (chara->random_force*1.5);
    }else{
        chara->angle += (chara->random_force*1.5);
    }
    
    if(key_state[ALLEGRO_KEY_LEFT]){
        chara->angle -= chara->balance_speed;
    }else if(key_state[ALLEGRO_KEY_RIGHT]){
        chara->angle += chara->balance_speed;
    }
    
    // 如果角度大於45度，精神值減少，當精神值歸零game over
    while((chara->angle)>360 || (chara->angle)<-360){
        if((chara->angle)>360){
            chara->angle -= 360;
        }else if((chara->angle)<-360){
            chara->angle += 360;
        }
    }
    //printf("Angle: %f Spirit: %d\n", chara->angle, chara->spirit);
    if(chara->angle > 70 || chara->angle < -70)
    {
        chara->spirit -= 0.1;
        if(chara->spirit <0) chara->spirit = 0;
    }
}
void Character_SEA_draw(Elements *self)
{
    // with the state, draw corresponding image
    Character_SEA *chara = ((Character_SEA *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(chara->gif_Ichi, al_get_time());

    if (frame)
    {
        al_draw_rotated_bitmap(frame, al_get_bitmap_width(frame) / 2, al_get_bitmap_height(frame) / 2 ,chara->x, chara->y,chara->angle * ALLEGRO_PI / 180, ((chara->dir) ? ALLEGRO_FLIP_HORIZONTAL : 0));
    }
}
void Character_SEA_destory(Elements *self)
{
    Character_SEA *Obj = ((Character_SEA *)(self->pDerivedObj));
    algif_destroy_animation(Obj->gif_Ichi);
    free(Obj);
    free(self);
}

void _Character_SEA_update_position(Elements *self, int dx, int dy)
{
    Character_SEA *chara = ((Character_SEA *)(self->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
}

void Character_SEA_interact(Elements *self) {}
