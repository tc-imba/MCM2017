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
        cout << i << '\t' << speedDistance << '\t';


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
        data.lastEnterTime = 0;

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
        cout << data.cars[0].size() << '\t';


        // 计算下一辆车的偏移
        offsetDistance += data.mile - ((int) data.idealCount) * data.idealDistance;

        cout << endl;

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
        cout << now << endl;
        for (int j = 0; j < m_milepost.size(); j++)
        {
            bool addCarFlag = false;
            if (now - m_milepost[j].lastEnterTime >= m_milepost[j].idealPeriod)
            {
                m_milepost[j].lastEnterTime += m_milepost[j].idealPeriod;
                addCarFlag = true;
            }
            for (int k = 0; k < m_milepost[j].cars.size(); k++)
            {
                // 计算第一辆车
                bool deleteFlag = false;
                auto it = m_milepost[j].cars[k].begin();
                if (it == m_milepost[j].cars[k].end())continue;
                double distance;
                if (j == 0)
                {
                    deleteFlag = true;
                    distance = (*it)->idealDistance();
                }
                else if (m_milepost[j - 1].cars.size() <= k)
                {
                    deleteFlag = true;
                    distance = (*it)->idealDistance();
                }
                else if (m_milepost[j - 1].cars[k].size() == 0)
                {
                    distance = (*it)->idealDistance();
                }
                else
                {
                    auto front = m_milepost[j - 1].cars[k].back();
                    distance = m_milepost[j - 1].mile
                               - front->m_pos + (*it)->m_pos;
                }
                (*it)->move(distance, m_period);

                // 计算中间的车
                if (m_milepost[j].cars[k].size() > 1)
                {
                    double lastPos = (*it++)->m_pos;
                    for (; it != m_milepost[j].cars[k].end(); ++it)
                    {
                        (*it)->move((*it)->m_pos - lastPos, m_period);
                        lastPos = (*it)->m_pos;
                    }
                }

                // 判断第一辆车是否删除或并入前面
                auto front = m_milepost[j].cars[k].front();
                if (front->m_pos < 0)
                {
                    m_milepost[j].cars[k].pop_front();
                    if (deleteFlag)
                    {
                        delete front;
                    }
                    else
                    {
                        m_milepost[j - 1].cars[k].push_back(front);
                    }
                }

                // 加入新的车
                if (addCarFlag)
                {
                    bool newCarFlag = false;
                    if (j == m_milepost.size() - 1)newCarFlag = true;
                    if (m_milepost[j + 1].cars.size() <= k)newCarFlag = true;
                    if (newCarFlag)
                    {
                        addCar(m_milepost[j].cars[k], m_milepost[j].idealSpeed,
                                m_milepost[j].mile);
                    }
                }
            }
        }
    }
}


