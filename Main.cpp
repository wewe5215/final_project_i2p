#include "GameWindow.h"

int main(int argc, char *argv[])
{
    GameWindow *TowerGame= new GameWindow();
    srand(time(NULL));
    TowerGame->game_play();
    
    delete TowerGame;
    return 0;
}
