#include "sceneManager.h"
#include "menu.h"
#include "gamescene.h"
#include "gardenscene.h"
#include "seascene.h"
#include "gameoverscene.h"
#include "Gameclearscene.h"
#include "../global.h"

Scene *scene = NULL;
void create_scene(SceneType type)
{
    printf("[DEBUG] create_scene: type = %d\n", type);
    fflush(stdout);

    switch (type)
    {
    case Menu_L:
        scene = New_Menu(Menu_L, &g_status);
        printf("[DEBUG]  -> New_Menu returned %p\n", (void*)scene);
        break;
    case GameScene_L:
        scene = New_GameScene(GameScene_L, &g_status);
         printf("[DEBUG]  -> New_GameScene returned %p\n", (void*)scene);
        break;
    case GardenScene_L: 
        scene = New_GardenScene(GardenScene_L, &g_status); 
        printf("[DEBUG]  -> New_GardenScene returned %p\n", (void*)scene);
        break;
    case SeaScene_L: 
        scene = New_SEAScene(SeaScene_L, &g_status); 
        printf("[DEBUG]  -> New_SeaScene returned %p\n", (void*)scene);
        if (!scene) {
            printf("[ERROR] scene is NULL after New_SEAScene!\n");
        }
        else if (g_status.HP <= 0) {
            printf("[DEBUG] g_status dead，HP = %d\n", g_status.HP);
        }
        break;
    case GameOverScene_L:
        scene = New_GameOverScene(GameOverScene_L, &g_status);
        break;
    case GameclearScene_L:
        scene = New_GameclearScene(GameclearScene_L, &g_status);
        break;
    default:
        printf("[DEBUG]  -> unknown type!\n");
        break;
    }
    fflush(stdout);
}
