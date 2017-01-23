/*************************************************
AutoCar.cpp
**************************************************/

#include "AutoCar.h"
#include "Layout.h"

AutoCar::AutoCar(Layout *layout, int milepostNo) : Car(layout, milepostNo)
{
    m_respondTime = 0;
    m_chain = false;
}

AutoCar::~AutoCar()
{

}

double AutoCar::getAcceleration()
{
    return m_acceleration * m_state;
}

void AutoCar::move(double period)
{
    if (m_frontCar)
    {
        double deltaPos = 0;
        if (m_distance > idealDistance(period) * 1.1)
        {
            m_chain = false;
            Car::move(period);
            return;
        }

        if (m_speed > 20 && m_speed <= 40 && m_distance >= 20. / 5280. && m_distance <= 70. / 5280)
        {
            m_chain = true;
            deltaPos = 45. / 5280.;
        }
        else if (m_speed > 40 && m_distance >= 65. / 5280)
        {
            m_chain = true;
            deltaPos = 100. / 5280.;
        }

        if (m_chain)
        {
            double newPos = 0;
            if (m_milepostNo != m_frontCar->m_milepostNo)
            {
                double mile = 0;
                for (int i = m_frontCar->m_milepostNo; i < m_milepostNo; i++)
                {
                    mile += m_layout->m_milepost[i].mile;
                }
                newPos = -(mile - m_frontCar->m_pos) + deltaPos;
            }
            else
            {
                newPos = m_frontCar->m_pos + deltaPos;
            }
            m_speed = m_frontCar->m_speed;
            m_deltaPos += m_pos - newPos;
            m_pos = newPos;
            return;
        }

    }
    else
    {
        m_chain = false;
    }
    Car::move(period);
}