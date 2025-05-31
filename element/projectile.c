#include "projectile.h"
#include "tree.h"
#include "mushroom.h"
#include "../shapes/Circle.h"
#include "../scene/gamescene.h" // for element label
#include "../scene/sceneManager.h" // for scene variable
/*
   [Projectile function]
*/
Elements *New_Projectile(int label, int x, int y, int v)
{
    Projectile *pDerivedObj = (Projectile *)malloc(sizeof(Projectile));
    Elements *pObj = New_Elements(label);
    //新增起點
    if (!pDerivedObj || !pObj) {
        fprintf(stderr, "Failed to allocate memory\n");
        if (pDerivedObj) free(pDerivedObj);
        if (pObj) free(pObj);
        return NULL;
    }
    //
    // 載入 GIF
    pDerivedObj->gif = algif_load_animation("assets/image/shovel_mushroom.gif");
    if (!pDerivedObj->gif) {
        fprintf(stderr, "Failed to load GIF: assets/image/shovel_mushroom.gif\n");
        free(pDerivedObj);
        free(pObj);
        return NULL;
    }
    // setting derived object member
    //pDerivedObj->img = al_load_bitmap("assets/image/projectile.png");
    pDerivedObj->width = pDerivedObj->gif->width;
    pDerivedObj->height = pDerivedObj->gif->height;
    pDerivedObj->x = x;
    pDerivedObj->y = y;
    pDerivedObj->v = v;// GIF 設為 0 表示靜態
    pDerivedObj->current_frame = 0;
    pDerivedObj->timer = 0;
    //pDerivedObj->frame_delay =  pDerivedObj->gif->frames[0].duration / 1000.0f;
    pDerivedObj->done_delay = 0; // 完成後延遲
    pDerivedObj->done = false;
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x + pDerivedObj->width / 2,
                                     pDerivedObj->y + pDerivedObj->height / 2,
                                     min(pDerivedObj->width, pDerivedObj->height) / 2);
    
    // setting the interact object
    pObj->inter_obj[pObj->inter_len++] = Tree_L;
    pObj->inter_obj[pObj->inter_len++] = Mushroom_L;
    // setting derived object function函數指標
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Projectile_update;
    pObj->Interact = Projectile_interact;
    pObj->Draw = Projectile_draw;
    pObj->Destroy = Projectile_destory;

    return pObj;
}
void Projectile_update(Elements *self, int delta_time)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    ALGIF_FRAME *frame = &Obj->gif->frames[Obj->current_frame];
    float frame_duration = frame->duration / 100; // 毫秒轉秒
    if (Obj->timer >= frame_duration) {
        Obj->current_frame++;
        Obj->timer = 0;
        if (Obj->current_frame >= Obj->gif->frames_count) {
            Obj->current_frame = Obj->gif->frames_count - 1;
            Obj->done_delay += delta_time;
            if (Obj->done_delay >= 2) {
                Obj->done = true;
            }
        }
    }
    /*Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if (!Obj->done) {
        Obj->timer += delta_time;
        if (Obj->timer >= Obj->frame_delay) {
            Obj->current_frame++;
            Obj->timer = 0.0f;
            if (Obj->current_frame >= Obj->gif->frames_count) {
                Obj->done = true; // 播放完立即消失
            } 
        }
    }*/
    //_Projectile_update_position(self, Obj->v, 0);
}
void _Projectile_update_position(Elements *self, int dx, int dy)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    Obj->x += dx;
    Obj->y += dy;
    Shape *hitbox = Obj->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}
void Projectile_interact(Elements *self)
{
    for (int j = 0; j < self->inter_len; j++)
    {
        int inter_label = self->inter_obj[j];
        ElementVec labelEle = _Get_label_elements(scene, inter_label);
        for (int i = 0; i < labelEle.len; i++)
        {
            if (inter_label == Mushroom_L)
            {
                _Projectile_interact_Mushroom(self, labelEle.arr[i]);
            }
            else if (inter_label == Tree_L)
            {
                _Projectile_interact_Tree(self, labelEle.arr[i]);
            }
        }
    }
}
void _Projectile_interact_Mushroom(Elements *self, Elements *tar)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if (Obj->x < 0 - Obj->width)
        self->dele = true;
    else if (Obj->x > WIDTH + Obj->width)
        self->dele = true;
}
/*void _Projectile_interact_Mushroom(Elements *self, Elements *tar)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    Mushroom *mush = ((Mushroom *)(tar->pDerivedObj));
    static ALLEGRO_SAMPLE *eat_sound = NULL;
    if (mush->active && !Obj->done)
    {
        // 圓形碰撞檢測
        Circle *proj_hitbox = Obj->hitbox;
        Circle *mush_hitbox = mush->hitbox;
        
        // 更新 hitbox 中心點（隨物件移動）
        proj_hitbox->x = Obj->x + Obj->width / 2;
        proj_hitbox->y = Obj->y + Obj->height / 2;
        mush_hitbox->x = mush->x + mush->width / 2;
        mush_hitbox->y = mush->y + mush->height / 2;

        // 計算兩圓心之間的距離
        float dx = proj_hitbox->x - mush_hitbox->x;
        float dy = proj_hitbox->y - mush_hitbox->y;
        float distance = sqrt(dx * dx + dy * dy);

        // 如果距離小於兩半徑之和，則碰撞
        if (distance <= (proj_hitbox->r + mush_hitbox->r))
        {
            Obj->done = true;    // 投射物完成
            mush->active = false; // 蘑菇消失
            al_play_sample(eat_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
    }
}*/
void _Projectile_interact_Tree(Elements *self, Elements *tar)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    Tree *tree = ((Tree *)(tar->pDerivedObj));
    if (tree->hitbox->overlap(tree->hitbox, Obj->hitbox))
    {
        self->dele = true;
    }
}
void Projectile_draw(Elements *self)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if (!Obj->done && Obj->gif){
        ALLEGRO_BITMAP *frame_bitmap = algif_get_bitmap(Obj->gif, Obj->current_frame);
        if (frame_bitmap) {
            al_draw_bitmap(frame_bitmap, Obj->x, Obj->y, 0);
        }
    }
}
void Projectile_destory(Elements *self)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    algif_destroy_animation(Obj->gif);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
