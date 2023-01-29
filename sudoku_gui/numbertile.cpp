#include "numbertile.h"
#include <random>

numbertile::numbertile():
    number_(0), numberShown_(0)
{
    pNums_ = std::vector<int>();

    for (int x = 1; x <= 9; x ++)
    {
        pNums_.push_back(x);
    }

}

int numbertile::getAmount() const
{
    if (pNums_.size() == 0)
    {
        return 11;
    }
    return pNums_.size();
}

void numbertile::setNum(int seed)
{
    if (pNums_.size() == 0){return;}
    std::default_random_engine randomEng;
    int max = pNums_.size() - 1;
    std::uniform_int_distribution<int> distribution = std::uniform_int_distribution<int>(0, max);
    randomEng.seed(seed);

    // Wiping out the first random number (which is almost almost 0)
    distribution(randomEng);
    number_ = pNums_.at(distribution(randomEng));
    pNums_.clear();
}

void numbertile::removeNum(int num)
{
    if (pNums_.size() == 0){return;}
    std::vector<int>::iterator poistettava = std::find(pNums_.begin(), pNums_.end(), num);
    if (poistettava == pNums_.end())
    {
        return;
    }
    pNums_.erase(poistettava);
}

int numbertile::getNum() const
{
    return number_;
}

void numbertile::setShownNumber(int number, bool stat)
{
    if (number_ == 0)
    {
        return;
    }
    numberShown_ = number;
    if (stat)
    {
        number_ = 0;
    }

}

int numbertile::getShownNum() const
{
    return numberShown_;
}

void numbertile::eraseNumber()
{
    if (number_ == 0)
    {
        return;
    }
    numberShown_ = 0;
}

void numbertile::reset()
{
    number_ = 0;
    numberShown_ = 0;

    pNums_.clear();

    for (int x = 1; x <= 9; x ++)
    {
        pNums_.push_back(x);
    }
}

void numbertile::setNumMan(int num)
{
    number_ = num;
    numberShown_ = num;
}




