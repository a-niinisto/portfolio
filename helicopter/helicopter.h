#ifndef HELICOPTER_H
#define HELICOPTER_H
#include <vector>

const int ACCELMAX = 20;
const int SPEEDMAX = 40;
const int ROPELENGTH = 20;

using Coords = std::pair<int,int>;

class helicopter
{
public:
    helicopter();

    Coords getLocation() const;

    void changeAccel(Coords dir);

    int getXAccel() const;

    Coords getRopeLoc() const;


private:
    void changeSpeed();

    void changeLocation();

    void calcRopeLoc();
    Coords loc_;
    Coords speed_;
    Coords accel_;

    double ropeAngle_;
};

#endif // HELICOPTER_H
