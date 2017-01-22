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

        m_milepost.push_back(Milepost());
        Milepost &data = m_milepost.back();

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
            Car *frontCar = NULL;
            for (int k = 1; k <= data.idealCount; k++)
            {
                auto newCar = addCar(carList, data.idealSpeed, k * data.idealDistance - offsetDistance, i);
                if (k == 1)
                {
                    if (i == 0) frontCar = NULL;
                    else if (m_milepost[i - 1].cars.size() <= j) frontCar = NULL;
                    else frontCar = m_milepost[i - 1].cars[j].back();
                }
                if (frontCar)frontCar->m_backCar = newCar;
                newCar->m_frontCar = frontCar;
                frontCar = newCar;

                //cout << carList.back()->m_pos << '\t';
            }
            //cout << j << "\t" << carList.size() << endl;
            data.cars.push_back(carList);
        }


        cout << data.cars[0].size() << '\t';


        // 计算下一辆车的偏移
        offsetDistance += data.mile - ((int) data.idealCount) * data.idealDistance;

        cout << endl;

    }

}

Car *Layout::addCar(std::list<Car *> &carQueue, double speed, double pos, int milepostNo)
{
    Car *car;
    if (((double) rand()) / RAND_MAX < m_autoPercentage)
    {
        car = new AutoCar(this, milepostNo);
    }
    else
    {
        car = new HumanCar(this, milepostNo);
    }
    car->m_speed = speed;
    car->m_pos = pos;
    carQueue.push_back(car);
    return car;
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
                if ((*it)->m_distance <= 0)
                {
                    cout << i << "\t" << j << "\t" << k << "\t"
                         << 1 << "/" << m_milepost[j].cars[k].size()
                         << "\t" << (*it)->m_distance << endl;
                    auto car = (*it);
                    cout << m_milepost[j].mile << endl;
                    for (int i = 0; i < 10; i++)
                    {
                        if (!car->m_frontCar)break;
                        cout << car->m_milepostNo << "\t" << car->m_pos << endl;
                        car = car->m_frontCar;
                    }
                }


                // 计算后面的车
                if (m_milepost[j].cars[k].size() > 1)
                {
                    int index = 1;
                    for (double lastPos = (*it++)->m_pos; it != m_milepost[j].cars[k].end(); ++it)
                    {
                        //(*it)->move((*it)->m_pos - lastPos, m_period);
                        (*it)->m_distance = (*it)->m_pos - lastPos;
                        if ((*it)->m_distance <= 0)
                        {
                            cout << i << "\t" << j << "\t" << k << "\t"
                                 << index + 1 << "/" << m_milepost[j].cars[k].size()
                                 << "\t" << (*it)->m_distance << endl;
                            auto car = (*it);
                            cout << m_milepost[j].mile << endl;
                            for (int i = 0; i < 10; i++)
                            {
                                if (!car->m_frontCar)break;
                                cout << car->m_milepostNo << "\t" << car->m_pos << endl;
                                car = car->m_frontCar;
                            }
                        }
                        if ((*it)->m_pos > m_milepost[j].mile)
                        {
                            cout << i << "\t" << j << "\t" << k << "\t"
                                 << index + 1 << "/" << m_milepost[j].cars[k].size()
                                 << "\t" << (*it)->m_pos << "/" << m_milepost[j].mile << endl;
                        }
                        lastPos = (*it)->m_pos;
                        index++;
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
                    //cout  << i << setw(4) << j << endl;
                    it->move(m_period);
                }

                // 判断第一辆车是否删除或并入前面
                auto front = m_milepost[j].cars[k].front();
                else if (front->m_pos < 0)
                {
                    m_milepost[j].cars[k].pop_front();
                    if (!front->m_frontCar)
                    {
                        if (front->m_backCar)front->m_backCar->m_frontCar = NULL;
                        delete front;
                    }
                    else
                    {
                        m_milepost[j - 1].cars[k].push_back(front);
                        front->m_pos += m_milepost[j - 1].mile;
                        if (front->m_milepostNo != j)
                        {
                            cout << j << endl;
                        }
                        front->m_milepostNo = j - 1;
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
                        auto frontCar = m_milepost[j].cars[k].back();
                        if (frontCar)
                        {
                            if (frontCar->m_pos + 20. / 5280. > m_milepost[j].mile)
                            {
                                continue;
                            }
                        }
                        auto newCar = addCar(m_milepost[j].cars[k],
                                m_milepost[j].idealSpeed, m_milepost[j].mile, j);
                        if (frontCar) newCar->m_backCar = frontCar->m_backCar;
                        if (newCar->m_backCar)newCar->m_backCar->m_frontCar = newCar;
                        newCar->m_frontCar = frontCar;
                        if (frontCar) frontCar->m_backCar = newCar;
                    }
                }
            }
        }

        if (i % 10 == 0)
        {
            //cout << i << endl;
            printSpeed();
        }

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
