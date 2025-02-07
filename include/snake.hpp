#pragma once
#include <array>
#include <deque>
#include <ncurses.h>
#include <random>

class GameSnake {
    public:
        GameSnake ();
        ~GameSnake();

        void start() const;

    private:
        void drawBox(const int &maxX, const int &maxY) const;
        void drawScore() const;
        void drawFood() const;
        void drawSnake() const;
        void drawGameOver() const;

        struct coors {
            int x;
            int y;
        };

        mutable WINDOW *currWin = nullptr;
        mutable coors foodPostion = {0, 0};
        mutable std::deque<std::array<int, 2>> snake {};
        const std::array<int, 4> directions = {KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP};

        mutable int currDirection = KEY_RIGHT;
        mutable int inverseDirection = KEY_LEFT;
        mutable bool gameOver = false;
        mutable int score = 0;

        mutable std::mt19937 gen;
        mutable std::uniform_int_distribution<int> distX;
        mutable std::uniform_int_distribution<int> distY;
};

