//
// Created by liu on 2017/1/21.
//

#include "Layout.h"

Layout::Layout(const vector <Data> &m_data, bool isNormal, bool isASC, double period)
{
    m_period = period;

    double offsetDistance = 0.;

    for (int i = 0; i < m_data.size(); i++)
    {
        int paneNum = isASC ? m_data[i].paneASC : m_data[i].paneDESC;
        double traffic = m_data[i].traffic * (isNormal ? 0.02 : 0.04);
        auto speedDistance = getIdealSpeedDistance(traffic, paneNum);
        cout << i << "\t" << speedDistance << endl;


        Milepost data;
        data.idealSpeed = speedDistance.first;
        data.idealDistance = speedDistance.second;
        data.mile = m_data[i].end - m_data[i].start;

        // 前一段区域最后多出的距离超过本段理想距离，直接放置在起点
        if (offsetDistance > data.idealDistance)
        {
            offsetDistance = 0;
        }

        data.idealCount = (data.mile + offsetDistance) / data.idealDistance;
        data.idealPeriod = data.idealDistance / data.idealSpeed * 3600; // in seconds

        for (int j = 0; j < paneNum; j++)
        {
            std::list<Car *> carList;
            for (int k = 1; k <= data.idealCount; k++)
            {
                addCar(carList, data.idealSpeed,
                        k * data.idealDistance - offsetDistance);
            }
            //cout << j << "\t" << carList.size() << endl;
            data.cars.push_back(carList);
        }
        m_milepost.push_back(data);

        // 计算下一辆车的偏移
        offsetDistance += data.mile - ((int) data.idealCount) * data.idealDistance;


        //cout << offsetDistance << endl;

    }

}

void Layout::addCar(std::list<Car *> &carQueue, double speed, double pos)
{
    Car *car = new HumanCar();
    car->m_speed = speed;
    car->m_pos = pos;
    carQueue.push_back(car);
}

void Layout::simulate(double time)
{
    for (int i = 0; i < time / m_period; i++)
    {
        double now = i * m_period;
        for (int j = 0; j < m_milepost.size(); j++)
        {
            for (int k = 0; k < m_milepost[j].cars.size(); k++)
            {

            }
        }
        /*bool addCarFlag = false;
        if (now - lastEnterTime >= idealPeriod)
        {
            lastEnterTime += idealPeriod;
            addCarFlag = true;
        }
        for (int j = 0; j < paneNum; j++)
        {
            if (addCarFlag)
            {
                addCar(carQueue[j], speed, milepost);
            }
            tick(carQueue[j]);
        }*/

    }
}


