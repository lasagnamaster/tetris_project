#include "game.cpp"
#include "tetromino.cpp"
#include <vector>

using namespace sf;

int main()
{
    Game game;

    while (game.running())
    {
        game.update();
        game.render();
    }

    return 0;
}