/*************************************************
HumanCar.h
**************************************************/

#ifndef MCM2017_HUMANCAR_H
#define MCM2017_HUMANCAR_H

#include "Car.h"

class HumanCar : public Car
{
public:
    HumanCar(Layout *layout, int milepostNo);

    ~HumanCar();

    double getAcceleration();

};


#endif //MCM2017_HUMANCAR_H
