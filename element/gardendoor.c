#include "gardendoor.h"
#include "../shapes/Rectangle.h"
#include "../scene/sceneManager.h"
#include "../global.h"  
#include "../audio/audio.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> 
#include "../scene/sceneManager.h" // for scene variable
#include <stdlib.h>
#define DEBUG_HITBOX 0          // <== 開啟 hitbox debug 模式
#define MIN_Y_HEIGHT_RATIO 0.7 // 新上界比例，範圍 0 到 1
#define HEIGHT 720            // 假設螢幕高度

extern Scene *scene;
/*
   [Gardendoor function]
*/
Elements *New_Gardendoor(int label)
{
    
    Gardendoor *pDerivedObj = (Gardendoor *)malloc(sizeof(Gardendoor));
    Elements *pObj = New_Elements(label);
    
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/garden_door.png");
    if (!pDerivedObj->img) {
        fprintf(stderr, "Failed to load assets/image/garden_door.png\n");
        exit(1);
    }
    printf("[DEBUG] garden_door.png loaded OK\n");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);

    pDerivedObj->x = rand() % (WIDTH - pDerivedObj->width);
    int min_y = HEIGHT * MIN_Y_HEIGHT_RATIO;               // 新上界
    int max_y = HEIGHT - pDerivedObj->height;              // 下界
    pDerivedObj->y = (rand() % (max_y - min_y + 1)) + min_y;

    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y ,
                                        pDerivedObj->x + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y +pDerivedObj->height);
    
    if (!pDerivedObj->hitbox) {
        fprintf(stderr, "[ERROR] Failed to create hitbox for Gardendoor\n");
        exit(1);
    }
    // setting derived object function
    pDerivedObj->active = true;
    
    pObj->pDerivedObj = pDerivedObj;
    if (!pObj) {
    fprintf(stderr, "[ERROR] New_Elements() failed in New_Gardendoor()\n");
    exit(1);
    }
    pObj->Update = Gardendoor_update;
    pObj->Interact = Gardendoor_interact;
    pObj->Draw = Gardendoor_draw;
    pObj->Destroy = Gardendoor_destroy;

    printf("[DEBUG] Gardendoor created at (%d, %d), label=%d\n", 
            pDerivedObj->x, pDerivedObj->y, label);

    return pObj;
}
void Gardendoor_update(Elements *self, float delta_time) {
    (void)delta_time;

    Gardendoor *gd = (Gardendoor *)self->pDerivedObj;
    /* 更新 hitbox 位置*/
    Rectangle *rect = (Rectangle *)(gd->hitbox->pDerivedObj);
    rect->x1 = gd->x + gd->width / 3;
    rect->y1 = gd->y ;
    rect->x2 = gd->x + 2 * gd->width / 3;
    rect->y2 = gd->y + gd->height; 

    if (gd->x + gd->width <= 1) {
        printf("gardendoor auto delete at x = %d\n", gd->x);
        self->dele = true;
    }
    
}
void Gardendoor_interact(Elements *self) {
    Gardendoor *door = (Gardendoor *)(self->pDerivedObj);
    
    /*printf("[DEBUG] Gardendoor_interact() called: mouse=(%f,%f), door=(%d,%d,%d,%d)\n",
           mouse.x, mouse.y,
           door->x, door->y,
           door->x + door->width, door->y + door->height);*/

    if (mouse_state[1] &&
        mouse.x >= door->x && mouse.x <= door->x + door->width &&
        mouse.y >= door->y && mouse.y <= door->y + door->height) 
    {
        printf("[GARDEN] Door clicked! Entering garden...\n");
        stop_bgm();
        window = GardenScene_L; // GardenScene_L = 2;
        scene->scene_end = true; // 切換場景
        
    }
    mouse_state[1] = false;
}

void Gardendoor_draw(Elements *self)
{
    Gardendoor *Obj = ((Gardendoor *)(self->pDerivedObj));
    if (!Obj->active) return;
    al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
    //al_draw_circle(mush->hitbox->center_x, mush->hitbox->center_y,  al_map_rgb(0, 255, 0), 2);
    

    #if DEBUG_HITBOX
    if (Obj->hitbox && Obj->hitbox->pDerivedObj && Obj->hitbox->getType && Obj->hitbox->getType() == RECTANGLE) {
        Rectangle *rect = (Rectangle *)(Obj->hitbox->pDerivedObj);
        al_draw_rectangle(
            rect->x1, rect->y1,
            rect->x2, rect->y2,
            al_map_rgb(255, 0, 0 ),10
        );
    }
    #endif

}
void Gardendoor_destroy(Elements *self)
{
    Gardendoor *Obj = ((Gardendoor *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
