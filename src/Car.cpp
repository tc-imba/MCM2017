/*************************************************
Car.cpp
**************************************************/

#include "Car.h"
#include "Layout.h"

Car::Car(Layout *layout, int milepostNo)
{
    m_layout = layout;
    m_milepostNo = milepostNo;
    m_speed = 0;
    //m_acceleration = 6.;
    m_acceleration = 2.5 + 0.5 * ((double) rand()) / RAND_MAX;
    m_wait = 0;
    m_state = UNIFORM;
    m_stateNext = UNIFORM;
    m_distance = 0;
    m_deleteFlag = false;
    m_frontCar = m_backCar = NULL;
}

Car::~Car()
{

}

void Car::move(double period)
{
    if (m_wait >= 0)
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
    if (m_wait < 0 && m_state != m_stateNext)
    {
        m_state = m_stateNext;
    }
    auto newSpeed = max(0., min(60., m_speed + getAcceleration() * period));
    auto newPos = m_pos - 0.5 * (m_speed + newSpeed) * period / 3600.;
    if (m_frontCar)
    {
        if (m_milepostNo != m_frontCar->m_milepostNo)
        {
            double mile = 0;
            for (int i = m_frontCar->m_milepostNo; i < m_milepostNo; i++)
            {
                mile += m_layout->m_milepost[i].mile;
            }
            if (newPos + (mile - m_frontCar->m_pos) <= 20. / 5280.)
            {
                newPos = -(mile - m_frontCar->m_pos) + 20. / 5280.;
                newSpeed = (m_pos - newPos) / (period / 3600.);
                //cout << "test1\t" << newPos << "\t" << newSpeed << endl;
            }
        }
        else if (newPos - m_frontCar->m_pos <= 20. / 5280.)
        {
            newPos = m_frontCar->m_pos + 20. / 5280.;
            newSpeed = (m_pos - newPos) / (period / 3600.);

        }
    }
    if (newPos >= m_layout->m_milepost[m_milepostNo].mile)
    {
        //cout << m_milepostNo << "\t" << m_pos << "\t" << m_frontCar->m_pos << "\t" << newPos << "\t" << newSpeed << endl;
    }
    m_pos = newPos;
    m_speed = newSpeed;
    /*if (newSpeed < 1e-5)
    {
        cout << m_milepostNo << "\t" << m_pos << "\t" << m_frontCar->m_pos << "\t" << newPos << "\t" << newSpeed << endl;
    }*/
}

