// element_label.h
#ifndef ELEMENT_LABEL_H
#define ELEMENT_LABEL_H

typedef enum EleType {
    // 共用
    Character_L,
    Pause_button_L,

    // 主場景專用
    Mushroom_L,
    Vine_L,
    Tree_L,
    Gardendoor_L,
    Projectile_L,

    // 花園場景專用
    Character_Garden_L,
    Mousesign_L,
    Mushrooms_red_L,
    Mushrooms_blue_L,
    Mushrooms_green_L,
    Mushrooms_purple_L,
    Button_L,
    Continue_L,
    Legend_L,

    MAX_LABEL  // 總數，用不上也可留作防越界
    
} EleType;

#endif
