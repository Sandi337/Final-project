#include "sceneManager.h"
#include "menu.h"
#include "gamescene.h"
#include "gardenscene.h"
#include "../global.h"

Scene *scene = NULL;
void create_scene(SceneType type)
{
    printf("[DEBUG] create_scene: type = %d\n", type);
    fflush(stdout);

    switch (type)
    {
    case Menu_L:
        scene = New_Menu(Menu_L);
        printf("[DEBUG]  -> New_Menu returned %p\n", (void*)scene);
        break;
    case GameScene_L:
        scene = New_GameScene(GameScene_L);
         printf("[DEBUG]  -> New_GameScene returned %p\n", (void*)scene);
        break;
    case GardenScene_L: 
        scene = New_GardenScene(GardenScene_L); 
        printf("[DEBUG]  -> New_GardenScene returned %p\n", (void*)scene);
        break;
    case SeaScene_L: 
        scene = New_GardenScene(SeaScene_L); 
        printf("[DEBUG]  -> New_SeaScene returned %p\n", (void*)scene);
        break;
    default:
        printf("[DEBUG]  -> unknown type!\n");
        break;
    }
    fflush(stdout);
}
