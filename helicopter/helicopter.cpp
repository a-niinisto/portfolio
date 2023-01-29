#include "helicopter.h"
#include <cmath>
#include <iostream>
#include <cmath>

helicopter::helicopter():
    loc_({0,0}),
    speed_({0, 0}),
    accel_({0,0}),
    ropeAngle_(0)
{
}

Coords helicopter::getLocation() const
{
    return loc_;
}

void helicopter::changeSpeed()
{
    speed_.first += accel_.first - speed_.first / abs(speed_.first)*
            int(pow(speed_.first + 1,2) * ACCELMAX / pow(SPEEDMAX, 2));
    speed_.second += accel_.second - speed_.second / abs(speed_.second)*
            int(pow(speed_.second + 1, 2) * ACCELMAX / pow(SPEEDMAX, 2));
    //speed_.first += accel_.first - int((speed_.first + 1) * ACCELMAX / (SPEEDMAX + 40));
    //speed_.second += accel_.second - int((speed_.second + 1) * ACCELMAX / (SPEEDMAX + 40));
}

void helicopter::changeLocation()
{
    loc_.first += speed_.first;
    loc_.second += speed_.second;
}

void helicopter::calcRopeLoc()
{
    //double accelCompl = accel_.second - speed_.second / abs(speed_.second)*
            //pow(speed_.second + 1, 2) * ACCELMAX / pow(SPEEDMAX, 2);
    double denom = -accel_.second - speed_.second / abs(speed_.second)*
            int(pow(speed_.second + 1, 2) * ACCELMAX / pow(SPEEDMAX, 2))/2;
    ropeAngle_ = denom / (2 * ACCELMAX) * M_PI/3;
    //std::cout << ": " << accelCompl << std::endl;
    std::cout << ropeAngle_ << std::endl;
}

void helicopter::changeAccel(Coords dir)
{
    if (dir.first == 0 and accel_.first != 0)
    {
        accel_.first -= accel_.first / std::abs(accel_.first);
    }
    if (dir.second == 0 and accel_.second != 0)
    {
        accel_.second -= accel_.second / std::abs(accel_.second);
    }
    if (std::abs(accel_.first) < ACCELMAX)
    {
        accel_.first += dir.first;
    }
    if (std::abs(accel_.second) < ACCELMAX)
    {
        accel_.second += dir.second;
    }
    changeSpeed();
    changeLocation();
    calcRopeLoc();
}

int helicopter::getXAccel() const
{
    return accel_.second;
}

Coords helicopter::getRopeLoc() const
{
    Coords loc;
    loc.first = cos(ropeAngle_) * ROPELENGTH;
    loc.second = sin(ropeAngle_) * ROPELENGTH;
    return loc;
}
