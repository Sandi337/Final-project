#ifndef Projectile_H_INCLUDED
#define Projectile_H_INCLUDED
#include "../global.h"
#include "element.h"
#include "../algif5/algif.h"
#include "../shapes/Shape.h"
/*
   [Projectile object]
*/
typedef struct _Projectile
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    int v;             // the velocity of projectile
    //ALLEGRO_BITMAP *img;
    ALGIF_ANIMATION *gif;
    Shape *hitbox; // the hitbox of object
    //新增gif的變數
    int current_frame;          // 當前 GIF 幀
    float timer;                // 計時器
    float frame_delay;          // 每幀延遲
    float done_delay;          // 每幀延遲
    bool done;                  // GIF播放完成標誌
} Projectile;
Elements *New_Projectile(int label, int x, int y, int v);
void Projectile_update(Elements *self, int delta_time);
void Projectile_interact(Elements *self);
void Projectile_draw(Elements *self);
void Projectile_destory(Elements *self);
void _Projectile_update_position(Elements *self, int dx, int dy);
void _Projectile_interact_Mushroom(Elements *self, Elements *tar);
void _Projectile_interact_Tree(Elements *self, Elements *tar);

#endif
