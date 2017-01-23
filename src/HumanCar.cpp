/*************************************************
HumanCar.cpp
**************************************************/

#include "HumanCar.h"

HumanCar::HumanCar(Layout *layout, int milepostNo) : Car(layout, milepostNo)
{
    m_respondTime = 1.;
}

HumanCar::~HumanCar()
{

}

double HumanCar::getAcceleration()
{
    return m_acceleration * m_state;
}