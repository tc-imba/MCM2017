//
// Created by liu on 2017/1/20.
//

#include "Car.h"


Car::Car()
{
    m_speed = 0;
    m_acceleration = 6.;
    m_wait = 0;
    m_state = UNIFORM;
    m_stateNext = UNIFORM;
}

Car::~Car()
{

}

void Car::move(double distance, double period)
{
    if (m_wait > 0)
    {
        m_wait--;
    }
    else
    {
        auto d = idealDistance();
        if (d < distance)
        {
            m_wait = int(m_respondTime / period);
            m_stateNext = ACCELERATE;
        }
        else if (d > distance)
        {
            m_wait = int(m_respondTime / period);
            m_stateNext = DECELERATE;
        }
    }
    if (m_wait <= 0 && m_state != m_stateNext)
    {
        m_state = m_stateNext;
    }
    m_pos -= (m_speed * period + 0.5 * m_acceleration * m_state * period * period);
    m_speed += m_acceleration * m_state * period;
}

