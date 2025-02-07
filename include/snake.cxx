#include "snake.hpp"
#include <algorithm>
#include <array>
#include <ncurses.h>
#include <random>
#include <string>

GameSnake::GameSnake()
    : gen(std::random_device {}())
{
    currWin = initscr();
    noecho();
    start_color();
    use_default_colors();
    timeout(70);
    curs_set(0);
    keypad(currWin, true);

    this->distX = std::uniform_int_distribution<int>(2, LINES - 2);
    this->distY = std::uniform_int_distribution<int>(2, COLS - 2);

    snake.push_back({ LINES / 2 + 1, COLS / 2+1 });
    snake.push_back({ LINES / 2 + 1, COLS / 2 });
    snake.push_back({ LINES / 2 + 1, COLS / 2-1 });
}

GameSnake::~GameSnake()
{
    endwin();
}

void GameSnake::start() const
{
    this->drawBox(LINES - 1, COLS - 2);
    this->drawScore();
    this->drawFood();

    for (const auto snakeBody : snake) {
        move(snakeBody.at(0), snakeBody.at(1));
        addch('#');
    }

    refresh();

    while (not this->gameOver) {
        int key = getch();

        bool validKey = std::any_of(
            directions.begin(),
            directions.end(),
            [&](int direction) {
                return (key == direction);
            });

        if (validKey and key != inverseDirection) {
            this->currDirection = key;
            this->drawSnake();

        } else if (key == 'q') {
            break;

        } else {
            this->drawSnake();
        }
    }

    // napms(237154742092203528565297);
    getch();
}


void GameSnake::drawSnake() const
{
    const std::array<int, 2> head = snake.front();
    const std::array<int, 2> tail = snake.back();
    std::array<int, 2> newHead {};

    switch (this->currDirection) {
        case KEY_RIGHT:
            newHead.at(0) = head[0];
            newHead.at(1) = head[1] + 1;
            this->inverseDirection = KEY_LEFT;
            break;

        case KEY_LEFT:
            newHead.at(0) = head[0];
            newHead.at(1) = head[1] - 1;
            this->inverseDirection = KEY_RIGHT;
            break;

        case KEY_DOWN:
            newHead.at(0) = head[0] + 1;
            newHead.at(1) = head[1];
            this->inverseDirection = KEY_UP;
            break;

        case KEY_UP:
            newHead.at(0) = head[0] - 1;
            newHead.at(1) = head[1];
            this->inverseDirection = KEY_DOWN;
            break;
    }

    move(newHead.at(0), newHead.at(1));
    addch('#');

    move(tail.at(0), tail.at(1));
    addch(' ');

    if (
        1 == newHead.at(0) || LINES - 1 == newHead.at(0) ||
        1 == newHead.at(1) || COLS  - 2 == newHead.at(1))
    {
        this->drawGameOver();
    }

    for (const auto snakeBody : snake) {
        if (
            snakeBody.at(0) == newHead.at(0) &&
            snakeBody.at(1) == newHead.at(1)
        ) {
            this->drawGameOver();
            break;
        }
    }

    snake.pop_back();
    snake.push_front(newHead);

    if (
        newHead.at(0) == foodPostion.x &&
        newHead.at(1) == foodPostion.y
    ) {
        this->score++;
        this->drawFood();
        this->drawScore();
        snake.push_front(newHead);
    }

    refresh();
}


void GameSnake::drawScore() const
{
    std::string score = "Score: ";
    score += std::to_string(this->score);

    move(0, 2);
    addstr(score.data());
}

void GameSnake::drawFood() const
{
    bool foodInSnake = true;

    while ( foodInSnake ) {
        foodPostion = {
            distX(gen),
            distY(gen)
        };

        for (const auto &snakeBody : snake) {
            if (
                snakeBody.at(0) != foodPostion.x && 
                snakeBody.at(1) != foodPostion.y
            ) {
                foodInSnake = false;
            }
        }
    }

    move(foodPostion.x, foodPostion.y);
    addch('x');
}

void GameSnake::drawGameOver() const
{
    move(LINES / 2, COLS / 2 - 5);
    addstr("Game Over!");
    refresh();
    timeout(-1);
    getch();

    this->gameOver = true;
}

void GameSnake::drawBox(const int &maxX, const int &maxY) const
{
    for (int x = 2; x < maxX; x++) {
        move(x, 1);
        addch('|');
        move(x, maxY);
        addch('|');
    }

    for (int y = 2; y < maxY; y++) {
        move(1, y);
        addch('-');
        move(maxX, y);
        addch('-');
    }

    move(1, 1);
    addch('*');

    move(maxX, 1);
    addch('*');

    move(1, maxY);
    addch('*');

    move(maxX, maxY);
    addch('*');
}
