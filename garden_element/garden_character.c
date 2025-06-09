#include "garden_character.h"
#include "../global.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>
#include <stdio.h>
/*
   [Character function]
*/
Elements *New_Character_Garden(int label)
{
    Character *pDerivedObj = (Character *)malloc(sizeof(Character));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load character images
    char state_string[5][10] = {"stop", "up", "down", "left", "right"};
    for (int i = 0; i < 5; i++)
    {
        char buffer[50];
        sprintf(buffer, "assets/image/ichigo_%s.gif", state_string[i]);
        pDerivedObj->gif_status[i] = algif_new_gif(buffer, -1);
        if (!pDerivedObj->gif_status[i]) {
            fprintf(stderr, "Failed to load %s\n", buffer);
            exit(1);
        }
    }
    // load effective sound
    al_get_default_mixer();

    // initial the geometric information of character
    pDerivedObj->width = pDerivedObj->gif_status[0]->width;
    pDerivedObj->height = pDerivedObj->gif_status[0]->height;
    pDerivedObj->x = WIDTH/2-80;
    pDerivedObj->y = HEIGHT - pDerivedObj->height - 80;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
    pDerivedObj->dir = false; // true: face to right, false: face to left
    // initial the animation component
    pDerivedObj->state = STOP;
    pDerivedObj->new_proj = false;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Character_Garden_draw;
    pObj->Update = Character_Garden_update;
    pObj->Interact = Character_Garden_interact;
    pObj->Destroy = Character_Garden_destroy;
    return pObj;
}
void Character_Garden_update(Elements *self)
{
    // use the idea of finite state machine to deal with different state
    Character *chara = ((Character *)(self->pDerivedObj));
    if (key_state[ALLEGRO_KEY_UP])
        {
            chara->state = UP;
        }
    else if (key_state[ALLEGRO_KEY_DOWN])
        {
            chara->state = DOWN;
        }
    else if (key_state[ALLEGRO_KEY_LEFT])
        {
            chara->state = LEFT;
        }
    else if (key_state[ALLEGRO_KEY_RIGHT])
        {
            chara->state = RIGHT;
        }
    else
        {
            chara->state = STOP;
        }
    
}
void Character_Garden_draw(Elements *self)
{
    // with the state, draw corresponding image
    Character *chara = ((Character *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(chara->gif_status[chara->state], al_get_time());
    if (frame)
    {
        al_draw_bitmap(frame, chara->x, chara->y, ((chara->dir) ? ALLEGRO_FLIP_HORIZONTAL : 0));
    }
}
void Character_Garden_destroy(Elements *self)
{
    Character *Obj = ((Character *)(self->pDerivedObj));
    for (int i = 0; i < 5; i++)
        algif_destroy_animation(Obj->gif_status[i]);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}

void _Character_Garden_update_position(Elements *self, int dx, int dy)
{
    Character *chara = ((Character *)(self->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

void Character_Garden_interact(Elements *self) {}
