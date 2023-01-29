#include "gameboard.h"
#include <iostream>

gameboard::gameboard():
    solved_(false)
{
    initEmpty();
}


void gameboard::print() const
{
    for (unsigned int y = 0; y < 9; y++)
    {
        for (unsigned int x = 0; x < 9; x++)
        {
            std::cout << board_.at(y).at(x).getNum() << " ";
        }
        std::cout << std::endl;
    }
}

numbertile *gameboard::getTile(Coords cord)
{
    numbertile* tile = &board_.at(cord.first).at(cord.second);
    return tile;
}

bool gameboard::move(Coords square, int number)
{
    if (number == 0)
    {
        board_.at(square.first).at(square.second).eraseNumber();
    }
    else
    {
        board_.at(square.first).at(square.second).setShownNumber(number);
    }
    return checkWin();
}

void gameboard::initBoard(int seed, int showAmount)
{
    if (board_.at(0).at(0).getNum() != 0 or board_.at(0).at(0).getShownNum() != 0)
    {
        initEmpty();
    }
    randomEng_.seed(seed);
    distribution_ = std::uniform_int_distribution<int>(1, 300);

    // Wiping out the first random number (which is almost almost 0)
    distribution_(randomEng_);
    fill(board_);
    show(showAmount);
    solved_ = true;
}

void gameboard::resetBoard()
{
    for (unsigned int y = 0; y < 9; y++)
    {
        for (unsigned int x = 0; x < 9; x++)
        {
            board_.at(y).at(x).reset();
        }
    }
    solved_ = false;
}


bool gameboard::fill(std::vector<std::vector<numbertile> >& board)
{
    Coords leastC;
    if (board.at(4).at(4).getNum() == 0 and solved_ == false)
    {
        leastC.second = 4;
        leastC.first = 4;
    }
    else
    {
        numbertile* least = &board.at(0).at(0);
        leastC.second = 0;
        leastC.first = 0;
        for (unsigned int y = 0; y < 9; y++)
        {
            for (unsigned int x = 0; x < 9; x++)
            {
                int currentAm = board.at(y).at(x).getAmount();
                int currentNum = board.at(y).at(x).getNum();
                if (currentNum != 0)
                {
                    continue;
                }
                if (currentAm == 11)
                {
                    return false;
                }

                if (least->getAmount() > currentAm)
                {
                    least = &board.at(y).at(x);
                    leastC.second = x;
                    leastC.first = y;
                }
            }
        }
        if (least->getAmount() == 11)
        {
            return false;
        }
    }
    while (true)
    {
        std::vector<std::vector<numbertile>> boardProp = board;
        numbertile* least = &boardProp.at(leastC.first).at(leastC.second);
        int num = distribution_(randomEng_);

        least->setNum(num);
        if (board.at(leastC.first).at(leastC.second).getAmount() == 11)
        {
            return false;
        }
        int number = least->getNum();
        board.at(leastC.first).at(leastC.second).removeNum(number);
        int xStart = leastC.second - leastC.second % 3;
        int yStart = leastC.first - leastC.first % 3;
        for (int index = 0; index <= 8; index ++)
        {
            // sama sarake
            boardProp.at(index).at(leastC.second).removeNum(number);
            // sama rivi
            boardProp.at(leastC.first).at(index).removeNum(number);
            // sama neliö
            boardProp.at(yStart + index / 3).at(xStart + index % 3).removeNum(number);
        }
        if (checkReady(boardProp))
        {
            board_ = boardProp;
            return true;
        }
        else if (fill(boardProp))
        {
            return true;
        }
    }
}

bool gameboard::checkReady(const std::vector<std::vector<numbertile> > &board)
{
    for (unsigned int y = 0; y < 9; y++)
    {
        for (unsigned int x = 0; x < 9; x++)
        {
            if (board.at(y).at(x).getNum() == 0)
            {
              return false;
            }
        }
    }
    return true;
}

bool gameboard::checkWin() const
{
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            int numberRow = board_.at(y).at(x).getShownNum();
            int numberColumn = board_.at(x).at(y).getShownNum();
            if (numberRow == 0 or numberColumn == 0)
            {
                return false;
            }
        }
    }
    std::vector<int> row;
    std::vector<int> column;
    std::vector<int> square;
    for (int y = 0; y < 9; y++)
    {
        int xStart = y % 3 * 3;
        int yStart = y / 3 * 3;
        for (int x = 0; x < 9; x++)
        {
            int numberRow = board_.at(y).at(x).getShownNum();
            int numberColumn = board_.at(x).at(y).getShownNum();
            int numberSquare = board_.at(yStart + x / 3).at(xStart + x % 3).getShownNum();

            if (std::find(row.begin(), row.end(), numberRow) != row.end())
            {
                return false;
            }
            if (std::find(column.begin(), column.end(), numberColumn) != column.end())
            {
                return false;
            }
            if (std::find(square.begin(), square.end(), numberSquare) != square.end())
            {
                return false;
            }
            row.push_back(numberRow);
            column.push_back(numberColumn);
            square.push_back(numberSquare);
        }
        row.clear();
        column.clear();
        square.clear();
    }
    return true;
}

void gameboard::show(int amount)
{
    distribution_ = std::uniform_int_distribution<int>(0, 8);
    // Wiping out the first random number (which is almost almost 0)
    distribution_(randomEng_);
    int iteration = 0;
    while (iteration < amount)
    {
        numbertile* tile = &board_.at(distribution_(randomEng_)).at(distribution_(randomEng_));
        if (tile->getShownNum() == 0)
        {
            int num = tile->getNum();
            tile->setShownNumber(num, true);
            iteration ++;
        }
    }
}

void gameboard::initEmpty()
{
    board_.clear();
    for (int y = 0; y <= 8; y ++)
    {
        board_.push_back({});
        for (int x = 0; x <= 8; x ++)
        {
            board_.at(y).push_back(numbertile());
        }
    }
}

void gameboard::setNum(Coords square, int num)
{

     board_.at(square.first).at(square.second).setNumMan(num);

}

void gameboard::solve()
{
    if (solved_)
    {
        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {
                numbertile* current = &board_.at(y).at(x);
                int num = current->getNum();
                if (num != 0)
                {
                    current->setShownNumber(num, true);
                }
            }
        }
    }
    else
    {
        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {
                numbertile* current = &board_.at(y).at(x);
                int num = current->getShownNum();
                if (num != 0)
                {
                    Coords square = {y, x};
                    int xStart = square.second - square.second % 3;
                    int yStart = square.first - square.first % 3;
                    for (int index = 0; index <= 8; index ++)
                    {
                        // sama sarake
                        board_.at(index).at(square.second).removeNum(num);
                        // sama rivi
                        board_.at(square.first).at(index).removeNum(num);
                        // sama neliö
                        board_.at(yStart + index / 3).at(xStart + index % 3).removeNum(num);
                    }
                }

            }
        }
        solved_ = true;
        fill(board_);

        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {
                numbertile* current = &board_.at(y).at(x);
                int num = current->getNum();
                current->setShownNumber(num, true);
            }
        }
    }
}
