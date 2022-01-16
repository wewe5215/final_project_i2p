#include "GameWindow.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    GameWindow *TowerGame= new GameWindow();
    TowerGame->game_draw();
    TowerGame->game_start_event_loop();
    TowerGame->game_destroy();
    
    
    delete TowerGame;
    return 0;
}
