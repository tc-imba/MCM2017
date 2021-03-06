/*************************************************
Car.h
**************************************************/

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

    double m_distance;

    bool m_deleteFlag;

    STATE m_state, m_stateNext;

    Car *m_frontCar, *m_backCar;

    int m_milepostNo;

    double m_deltaPos;

    Layout *m_layout;

    Car(Layout *layout, int milepostNo);

    virtual ~Car() = 0;

    virtual void move(double period);

    virtual double getAcceleration() = 0;

    inline double idealDistance(double period)
    {
        if (m_speed < 20.)return 20. / 5280.;
        return m_speed * m_speed / (2 * BRAKE_ACCERLATION)
               + m_speed * m_respondTime / 3600. + 16. / 5280.;
    }
};


#endif //MCM2017_CAR_H
