#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include "numbertile.h"
#include <random>

using Coords = std::pair<int, int>;


class gameboard
{
public:
    gameboard();
    void print() const;

    numbertile* getTile(Coords cord);

    bool move(Coords square, int number);

    void initBoard(int seed, int showAmount);

    void resetBoard();



    // for solve
    void setNum(Coords square, int num);

    void solve();

private:
    bool fill(std::vector<std::vector<numbertile>>& board);
    bool checkReady(const std::vector<std::vector<numbertile>>& board);
    bool checkWin() const;
    void show(int amount);
    void initEmpty();

    std::vector<std::vector<numbertile>> board_;

    std::default_random_engine randomEng_;
    std::uniform_int_distribution<int> distribution_;

    bool solved_; //records whether the board already has set solution.
};

#endif // GAMEBOARD_H
