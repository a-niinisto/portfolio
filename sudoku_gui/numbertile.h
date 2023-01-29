#ifndef NUMBERTILE_H
#define NUMBERTILE_H

#include <vector>
#include <string>
#include <algorithm>

class numbertile
{
public:
    numbertile();

    // just for gameboard initialization
    int getAmount() const;
    void setNum(int seed);
    void removeNum(int num);


    int getNum() const;

    void setShownNumber(int number, bool stat = false);

    int getShownNum() const;

    void eraseNumber();

    void reset();

    // for solve:
    void setNumMan(int num);


private:

    int number_;
    std::vector<int> pNums_;
    int numberShown_;

};

#endif // NUMBERTILE_H
