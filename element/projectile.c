#include "projectile.h"
#include "tree.h"
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
    pDerivedObj->timer = 0.0f;
    pDerivedObj->frame_delay = pDerivedObj->gif->frames[0].duration / 100.0f; // 從 GIF 取得延遲
    pDerivedObj->done = false;
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x + pDerivedObj->width / 2,
                                     pDerivedObj->y + pDerivedObj->height / 2,
                                     min(pDerivedObj->width, pDerivedObj->height) / 2);
    
    // setting the interact object
    pObj->inter_obj[pObj->inter_len++] = Tree_L;
    pObj->inter_obj[pObj->inter_len++] = Floor_L;
    // setting derived object function函數指標
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Projectile_update;
    pObj->Interact = Projectile_interact;
    pObj->Draw = Projectile_draw;
    pObj->Destroy = Projectile_destory;

    return pObj;
}
void Projectile_update(Elements *self, float delta_time)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    Obj->timer += delta_time;
    if (Obj->timer >= Obj->frame_delay)
    {
        Obj->current_frame++;
        Obj->timer = 0.0f;
        if (Obj->current_frame >= Obj->gif->frames_count)
        {
            Obj->done = true;//gif 播放完成
        }else{
            Obj-> frame_delay = Obj->gif->frames[Obj->current_frame].duration / 100.0f;//更新延遲
        }
    }
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
            if (inter_label == Floor_L)
            {
                _Projectile_interact_Floor(self, labelEle.arr[i]);
            }
            else if (inter_label == Tree_L)
            {
                _Projectile_interact_Tree(self, labelEle.arr[i]);
            }
        }
    }
}
void _Projectile_interact_Floor(Elements *self, Elements *tar)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if (Obj->x < 0 - Obj->width)
        self->dele = true;
    else if (Obj->x > WIDTH + Obj->width)
        self->dele = true;
}
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
