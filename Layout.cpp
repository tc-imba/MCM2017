//
// Created by liu on 2017/1/21.
//

#include "Layout.h"

Layout::Layout(const vector <Data> &m_data, bool isNormal, bool isASC, double autoPercentage)
{
    m_autoPercentage = autoPercentage;

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
                //cout << carList.back()->m_pos << '\t';
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
    Car *car;
    if (((double) rand()) / RAND_MAX < m_autoPercentage)
    {
        car = new AutoCar();
    }
    else
    {
        car = new HumanCar();
    }
    car->m_speed = speed;
    car->m_pos = pos;
    carQueue.push_back(car);
}

void Layout::simulate(double time)
{
    openFiles();
    for (int i = 0; i < time / m_period; i++)
    {
        double now = i * m_period;
        for (int j = 0; j < m_milepost.size(); j++)
        {
            for (int k = 0; k < m_milepost[j].cars.size(); k++)
            {
                // 计算第一辆车
                auto it = m_milepost[j].cars[k].begin();
                if (it == m_milepost[j].cars[k].end())continue;
                (*it)->m_deleteFlag = false;
                double distance;
                if (j == 0)
                {
                    (*it)->m_deleteFlag = true;
                    distance = (*it)->idealDistance(m_period);
                }
                else if (m_milepost[j - 1].cars.size() <= k)
                {
                    (*it)->m_deleteFlag = true;
                    distance = (*it)->idealDistance(m_period);
                }
                else if (m_milepost[j - 1].cars[k].size() == 0)
                {
                    distance = (*it)->idealDistance(m_period);
                }
                else
                {
                    auto front = m_milepost[j - 1].cars[k].back();
                    distance = m_milepost[j - 1].mile
                               - front->m_pos + (*it)->m_pos;
                    //cout << j << '\t' << distance << '\t' << m_milepost[j - 1].mile << '\t' << front->m_pos << '\t' << (*it)->m_pos << '\t' << endl;
                }
                //(*it)->move(distance, m_period);
                (*it)->m_distance = distance;

                // 计算后面的车
                if (m_milepost[j].cars[k].size() > 1)
                {
                    double lastPos = (*it++)->m_pos;
                    for (; it != m_milepost[j].cars[k].end(); ++it)
                    {
                        //(*it)->move((*it)->m_pos - lastPos, m_period);
                        (*it)->m_distance = (*it)->m_pos - lastPos;
                        lastPos = (*it)->m_pos;
                    }
                }
            }
        }
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
                // 移动车
                for (auto it:m_milepost[j].cars[k])
                {
                    it->move(m_period);
                }

                // 判断第一辆车是否删除或并入前面
                auto front = m_milepost[j].cars[k].front();
                if (front->m_pos < 0)
                {
                    m_milepost[j].cars[k].pop_front();
                    if (front->m_deleteFlag)
                    {
                        delete front;
                    }
                    else
                    {
                        m_milepost[j - 1].cars[k].push_back(front);
                        front->m_pos += m_milepost[j - 1].mile;
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

        if (i % 10 == 0) printSpeed();

        /*
        //if (i % 10 != 0)continue;

        cout << now << endl;
        //for (int j = 0; j < m_milepost.size(); j++)
        for (int j = 0; j < 10; j++)
        {
            cout << j << '\t';
            for (int k = 0; k < m_milepost[j].cars.size(); k++)
            {
                double speedSum = 0;
                for (auto it:m_milepost[j].cars[k])
                {
                    //cout << it->m_speed << '\t';
                    speedSum += it->m_speed;
                }
                //cout << endl;
                double speedAvg = m_milepost[j].cars[k].size() > 0 ?
                                  speedSum / m_milepost[j].cars[k].size() : 0;
                cout << m_milepost[j].cars[k].size() << '\t' << speedAvg << '\t';
            }
            cout << endl;
        }
        //cout << endl;*/

    }
}

void Layout::openFiles()
{
    m_speed_file.open(m_outputPath + "speed.txt");
    if (!m_speed_file.is_open())
    {
        cerr << "Speed Data Failed to Open!" << endl << m_outputPath + "speed.txt";
    }
}

void Layout::closeFiles()
{
    m_speed_file.close();
}

void Layout::printSpeed()
{
    for (int i = 0; i < m_milepost.size(); i++)
    {
        for (int j = 0; j < m_milepost[i].cars.size(); j++)
        {
            m_speed_file << i << "\t" << j << "\t" << endl;
        }
        m_speed_file << endl;
    }
}
