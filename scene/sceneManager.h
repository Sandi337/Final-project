#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED
#include "scene.h"

extern Scene *scene;

typedef enum SceneType
{
    Menu_L = 0,
    GameScene_L = 1,
    GardenScene_L = 2,
    SeaScene_L = 3,
    GameOverScene_L = 4,
    GameclearScene_L = 6,
    Restart_L = 5  
} SceneType;
void create_scene(SceneType);

#endif