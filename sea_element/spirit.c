#include "spirit.h"
#include "charater_sea.h"
#include "../shapes/Circle.h"
#include "../scene/seascene.h" // for element label
#include "../scene/sceneManager.h" // for scene variable
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include "../scene/scene.h"
/*
   [Spirit function]
*/
Elements *New_Spirit(int label)
{
    Spirit *pDerivedObj = (Spirit *)malloc(sizeof(Spirit));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 14, 0);
    pDerivedObj->x = WIDTH/2 + 20;
    pDerivedObj->y = HEIGHT/2 + 10;
    pDerivedObj->r = 10;
    pDerivedObj-> time_interval = 10;
    pDerivedObj-> last_time = 0;
    pDerivedObj->color = al_map_rgb(0 , 255 , 0);
    // setting the interact object
    pObj->inter_obj[pObj->inter_len++] = Character_L;
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Spirit_update;
    pObj->Interact = Spirit_interact;
    pObj->Draw = Spirit_draw;
    pObj->Destroy = Spirit_destory;

    return pObj;
}
void Spirit_update(Elements *self, float delta_time)
{
    Spirit *Obj = ((Spirit *)(self->pDerivedObj)); 
}
void _Spirit_update_position(Elements *self, int dx, int dy)
{
    Spirit *Obj = ((Spirit *)(self->pDerivedObj));
    Obj->x += dx;
    Obj->y += dy;
}
void Spirit_interact(Elements *self)
{   
    ElementVec labelEle = _Get_label_elements(scene, Character_L);
    for (int i = 0; i < labelEle.len; i++){
        _Spirit_interact_Character(self, labelEle.arr[i]);
    }
    
}
void _Spirit_interact_Character(Elements *self, Elements *tar)
{
    Spirit *Obj = ((Spirit *)(self->pDerivedObj)); 
    Character_SEA *chara = (Character_SEA *)(tar->pDerivedObj);
    if (chara ->spirit > 60){   
        Obj -> color = al_map_rgb(0, 255, 0); // green
    }
    else if (chara -> spirit > 30){
        Obj -> color = al_map_rgb(255, 255, 0); //yellow
    }
    else if (chara -> spirit < 30) {
        Obj -> color = al_map_rgb(255, 0, 0); //red
    }
}
void Spirit_draw(Elements *self)
{
    Spirit *Obj = ((Spirit *)(self->pDerivedObj));
    al_draw_circle(Obj->x , Obj->y , Obj->r, Obj->color , 20);
    al_draw_text(Obj->font, al_map_rgb(255, 255, 255), WIDTH / 2 + 20, HEIGHT / 4 +120, ALLEGRO_ALIGN_CENTRE, "Spirit");
}
void Spirit_destory(Elements *self)
{
    Spirit *Obj = ((Spirit *)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    free(Obj);
    free(self);
}
