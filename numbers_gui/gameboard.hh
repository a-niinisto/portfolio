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
 *  Alkuperäinen tiedosto Tampereen yliopistolta.
 *
 *  Tiedoston muokkaaja:
 *  Nimi: Aaro Niinistö
 *  E-Mail: aaro.niinisto@aalto.fi
 *
 * */
#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include "numbertile.hh"
#include <vector>
#include <random>

const int SIZE = 4;
const int PRINT_WIDTH = 5;
const int NEW_VALUE = 2;
const int DEFAULT_GOAL = 2048;

class GameBoard
{
public:
    // Constructor
    GameBoard();

    // Destructor
    ~GameBoard();

    // Initializes the gameboard with nullptrs.
    void init_empty();

    // Initializes the random number generator and fills the gameboard
    // with random numbers or if isReset is true then, resets the gameboard
    // with random numbers.
    void fill(int seed, const bool isReset = false);

    // Draws a new location (coordinates) from the random number generator and
    // puts the NEW_VALUE on that location, unless check_if_empty is true and
    // the gameboard is full.
    void new_value(bool check_if_empty = true);

    // Returns true, if all the tiles in the game board are occupied,
    // otherwise returns false.
    bool is_full() const;

    // Prints the game board.
    void print() const;

    // Puts zero in to the designated tile which coordinates are given as parameter removable.
    void remove(Coords removable);

    // Gets points. Returns points as int-value.
    int getPoints();

    // Moves the number tiles in the gameboard, if possible (by calling
    // move method for each number tile).
    // Finally, resets turn of all number tiles.
    bool move(Coords dir, int goal);

    // Returns the element (number tile) in the given coordinates.
    NumberTile* get_item(Coords coords);

private:
    // Calculates the points:
    void calculatePoints();

    // Internal structure of the game board
    std::vector<std::vector<NumberTile*>> board_;

    // Random number generator and distribution,
    // they work better, if they are attributes of a class.
    std::default_random_engine randomEng_;
    std::uniform_int_distribution<int> distribution_;

    // Stores player's points:
    int points_;
};

#endif // GAMEBOARD_HH
