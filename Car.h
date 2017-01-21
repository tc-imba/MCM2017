//
// Created by liu on 2017/1/20.
//

#ifndef MCM2017_CAR_H
#define MCM2017_CAR_H

#include "mcm.h"

class Car
{
public:
    Car();

    virtual ~Car() = 0;

    double m_respondTime, m_speed, m_pos, m_accerlation;
};


#endif //MCM2017_CAR_H
