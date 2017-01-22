//
// Created by liu on 2017/1/20.
//

#include "HumanCar.h"

HumanCar::HumanCar() : Car()
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