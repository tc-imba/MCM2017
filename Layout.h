//
// Created by liu on 2017/1/21.
//

#ifndef MCM2017_LAYOUT_H
#define MCM2017_LAYOUT_H

#include "mcm.h"
#include "Car.h"
#include "HumanCar.h"
#include "AutoCar.h"

class Layout
{
public:
    struct stat
    {
        double speedPeriod, carTotal;
        int carPeriod, carFlow;
    };

    struct Milepost
    {
        double idealSpeed, idealDistance, idealPeriod, idealCount;
        double mile, lastEnterTime;
        std::vector<stat> statData;
        std::vector<std::list<Car *> > cars;
    };

    std::vector<Milepost> m_milepost;
    double m_period, m_autoPercentage;

    std::string m_outputPath;

    Layout(vector <Data> m_data, bool isNormal, bool isASC, double autoPercentage);

    Car *addCar(std::list<Car *> &carQueue, double speed, double pos, int milepostNo);

    void simulate(double time = 3600.);

    ofstream m_speed_file;

    bool openFile();

    void closeFile();

    void printSpeed(double interval);

};


#endif //MCM2017_LAYOUT_H
