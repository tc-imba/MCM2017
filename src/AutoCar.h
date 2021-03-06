/*************************************************
AutoCar.h
**************************************************/

#ifndef MCM2017_AUTOCAR_H
#define MCM2017_AUTOCAR_H

#include "Car.h"

class AutoCar : public Car
{
public:
    AutoCar(Layout *layout, int milepostNo);

    ~AutoCar();

    double getAcceleration();

    void move(double period);

    bool m_chain;
};


#endif //MCM2017_AUTOCAR_H
