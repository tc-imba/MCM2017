//
// Created by liu on 2017/1/22.
//

#ifndef MCM2017_AUTOCAR_H
#define MCM2017_AUTOCAR_H

#include "Car.h"

class AutoCar : public Car
{
public:
    AutoCar();

    ~AutoCar();

    double getAcceleration();

};


#endif //MCM2017_AUTOCAR_H
