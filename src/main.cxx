#include <ncurses.h>
#include "snake.hpp"

auto main() -> int
{
    const auto game = GameSnake();
    game.start();
}
