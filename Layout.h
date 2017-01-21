//
// Created by liu on 2017/1/21.
//

#ifndef MCM2017_LAYOUT_H
#define MCM2017_LAYOUT_H

#include "mcm.h"
#include "Car.h"
#include "HumanCar.h"

class Layout
{
public:
    struct Milepost
    {
        double idealSpeed, idealDistance, idealPeriod, idealCount;
        double mile, lastEnterTime;
        std::vector<std::list<Car *> > cars;
    };

    std::vector<Milepost> m_milepost;
    double m_period;

    Layout(const vector <Data> &m_data, bool isNormal, bool isASC, double period = 0.1);

    void addCar(std::list<Car *> &carQueue, double speed, double pos);

    void simulate(double time = 3600.);

};


#endif //MCM2017_LAYOUT_H
