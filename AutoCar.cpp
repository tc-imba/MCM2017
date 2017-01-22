//
// Created by liu on 2017/1/22.
//

#include "AutoCar.h"

AutoCar::AutoCar()
{
    m_respondTime = 0;
}

AutoCar::~AutoCar()
{

}

double AutoCar::getAcceleration()
{
    return m_acceleration * m_state;
}