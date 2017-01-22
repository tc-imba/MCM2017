//
// Created by liu on 2017/1/20.
//

#include "Car.h"


Car::Car()
{
    m_speed = 0;
    //m_acceleration = 6.;
    m_acceleration = 5. + 2. * ((double) rand()) / RAND_MAX;
    m_wait = 0;
    m_state = UNIFORM;
    m_stateNext = UNIFORM;
    m_distance = 0;
    m_deleteFlag = false;
}

Car::~Car()
{

}

void Car::move(double period)
{
    if (m_wait > 0)
    {
        m_wait--;
    }
    else
    {
        auto d = idealDistance(period);
        if (d * 1.1 < m_distance)
        {
            m_wait = int(m_respondTime / period);
            m_stateNext = ACCELERATE;
        }
        else if (d * 0.9 > m_distance)
        {
            m_wait = int(m_respondTime / period);
            m_stateNext = DECELERATE;
        }
    }
    if (m_wait <= 0 && m_state != m_stateNext)
    {
        m_state = m_stateNext;
    }
    auto newSpeed = max(0., min(60., m_speed + getAcceleration() * period));
    m_pos -= 0.5 * (m_speed + newSpeed) * period / 3600.;
    m_speed = newSpeed;
}

