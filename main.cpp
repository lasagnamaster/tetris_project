#include "game.cpp"
#include "tetromino.cpp"

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