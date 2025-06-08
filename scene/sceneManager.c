#include "sceneManager.h"
#include "menu.h"
#include "gamescene.h"
#include "gardenscene.h"

Scene *scene = NULL;
void create_scene(SceneType type)
{
    switch (type)
    {
    case Menu_L:
        scene = New_Menu(Menu_L);
        break;
    case GameScene_L:
        scene = New_GameScene(GameScene_L);
        break;
    case GardenScene_L: 
        scene = New_GardenScene(GardenScene_L); 
        break;
    case SeaScene_L: 
        scene = New_GardenScene(SeaScene_L); 
        break;
    default:
        break;
    }
}
