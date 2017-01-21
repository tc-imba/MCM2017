//
// Created by liu on 2017/1/20.
//

#ifndef MCM2017_CAR_H
#define MCM2017_CAR_H

#include "mcm.h"

class Car
{
public:
    enum STATE
    {
        UNIFORM = 0, ACCELERATE = 1, DECELERATE = -1, BRAKE = -2
    };

    double m_respondTime, m_speed, m_pos, m_acceleration;

    int m_wait;

    STATE m_state, m_stateNext;

    Car();

    virtual ~Car() = 0;

    virtual void move(double distance, double period);

    inline double idealDistance()
    {
        return m_speed * m_speed / (2 * BRAKE_ACCERLATION);
    }
};


#endif //MCM2017_CAR_H
