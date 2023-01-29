/*  COMP.CS.110 Project 4: game 2048
 *
 *  Gameboard-luokka toteuttaa pelin 2048.
 *
 *  Muokkaukset:
 *  Lisätty metodit remove, getPoints ja calculatePoints. Lisäksi muokattu
 *  metodia fill siten, että resetointi on mahdollista luomatta uusia numbertile-olioita että
 *  points_ resetoituu nollaksi.
 *  Lisätty myös attribuutti points_.
 *
 *
 *  Alkuperäinen tiedosto Tampereen yliopistolta.
 *
 *  Tiedoston muokkaaja:
 *  Nimi: Aaro Niinistö
 *  E-Mail: aaro.niinisto@aalto.fi
 *
 * */
#include "gameboard.hh"
#include <iostream>

GameBoard::GameBoard():
    points_(0)
{
}

GameBoard::~GameBoard()
{
    for(unsigned int y = 0; y < board_.size(); ++y)
    {
        for(unsigned int x = 0; x < board_.at(y).size(); ++x)
        {
            delete board_.at(x).at(y);
            board_.at(x).at(y) = nullptr;
        }
    }
}

void GameBoard::init_empty()
{
    std::vector<NumberTile*> row;
    for( int i = 0; i < SIZE; ++i)
    {
        row.push_back(nullptr);
    }
    for( int i = 0; i < SIZE; ++i)
    {
        board_.push_back(row);
    }
}

void GameBoard::fill(int seed, const bool isReset)
{
    randomEng_.seed(seed);
    distribution_ = std::uniform_int_distribution<int>(0, SIZE - 1);

    // Wiping out the first random number (which is almost almost 0)
    distribution_(randomEng_);

    if (not isReset)
    {
        for( auto y = 0; y < SIZE; ++y )
        {
            for( auto x = 0; x < SIZE; ++x )
            {
                board_.at(y).at(x) = new NumberTile(0, std::make_pair(y, x), this);
            }
        }
     }
    else
    {
        for( auto y = 0; y < SIZE; ++y )
        {
            for( auto x = 0; x < SIZE; ++x )
            {
                board_.at(y).at(x)->new_value(0);
            }
        }
    }

    for( int i = 0 ; i < SIZE ; ++i )
    {
        new_value();
    }
    points_ = 0;
}

void GameBoard::new_value(bool check_if_empty)
{
    if( check_if_empty and is_full() ){
        // So that we will not be stuck in a forever loop
        return;
    }
    int random_x = 0;
    int random_y = 0;
    do
    {
        random_x = distribution_(randomEng_);
        random_y = distribution_(randomEng_);
    } while( not board_.at(random_y).at(random_x)->new_value(NEW_VALUE) );
}

void GameBoard::print() const
{
    for( auto y : board_ )
    {
        std::cout << std::string(PRINT_WIDTH * SIZE + 1, '-') << std::endl;
        for( auto x : y )
        {
            x->print(PRINT_WIDTH);
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::string(PRINT_WIDTH * SIZE + 1, '-') << std::endl;
}

void GameBoard::remove(Coords removable)
{
    // place 0 as the value of the given tile:
    board_.at(removable.first).at(removable.second)->new_value(0);
}

int GameBoard::getPoints()
{
    return points_;
}

bool GameBoard::move(Coords dir, int goal)
{
    bool has_won = false;
    for( unsigned y = 0; y < board_.size(); ++y )
    {
        for( unsigned x = 0; x < board_.front().size(); ++x )
        {
            int directed_y = dir.first > 0 ? board_.size() - y - 1 : y;
            int directed_x = dir.second > 0 ? board_.back().size() - x - 1 : x;
            if( board_.at(directed_y).at(directed_x)->move(dir, goal) )
            {
                has_won = true;
            }
        }
    }
    for( auto &row : board_ )
    {
        for( auto &tile : row )
        {
            tile->reset_turn();
        }
    }
    calculatePoints();
    return has_won;
}

NumberTile* GameBoard::get_item(Coords coords)
{
    return board_.at(coords.first).at(coords.second);
}

void GameBoard::calculatePoints()
{
    int points = 0;
    for (unsigned int y = 0; y < board_.size(); y ++)
    {
        for( unsigned x = 0; x < board_.front().size(); x ++ )
        {
            int number = board_.at(y).at(x)->getNumber();
            if (number == 0 or number == 2)
            {
                continue;
            }
            points += 2*(number-1);
        }
    }
    points_ = points;
}

bool GameBoard::is_full() const
{
    for( auto y : board_ )
    {
        for( auto x : y )
        {
            if( x->is_empty() )
            {
                return false;
            }
        }
    }
    return true;
}
