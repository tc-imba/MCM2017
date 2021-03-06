/*************************************************
Layout.cpp
**************************************************/

#include "Layout.h"

Layout::Layout(vector <Data> m_data, bool isNormal, bool isASC, double autoPercentage)
{
    m_autoPercentage = autoPercentage;
    m_ban = false;
    double offsetDistance = 0.;
    if (isASC)
    {
        std::reverse(m_data.begin(), m_data.end());
    }

    for (int i = 0; i < m_data.size(); i++)
    {
        int paneNum = isASC ? m_data[i].paneASC : m_data[i].paneDESC;
        double traffic = m_data[i].traffic * (isNormal ? 0.02 : 0.04);
        auto speedDistance = getIdealSpeedDistance(traffic, paneNum);
        //cout << i << '\t' << speedDistance << '\t';

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
            data.statData.push_back(stat());
        }


        //cout << data.cars[0].size() << '\t';


        // 计算下一辆车的偏移
        offsetDistance += data.mile - ((int) data.idealCount) * data.idealDistance;

        //cout << endl;

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
    const int maxI = int(time / m_period + 1);
    for (int i = 0; i < 51; i++)
    {
        cout << '*';
    }
    cout << endl << '>';

    for (int i = 0; i <= maxI; i++)
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
                /*if ((*it)->m_distance <= 0)
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
                }*/


                // 计算后面的车
                if (m_milepost[j].cars[k].size() > 1)
                {
                    int index = 1;
                    for (double lastPos = (*it++)->m_pos; it != m_milepost[j].cars[k].end(); ++it)
                    {
                        //(*it)->move((*it)->m_pos - lastPos, m_period);
                        (*it)->m_distance = (*it)->m_pos - lastPos;
                        /*if ((*it)->m_distance <= 0)
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
                        }*/
                        /*if ((*it)->m_pos > m_milepost[j].mile)
                        {
                            cout << i << "\t" << j << "\t" << k << "\t"
                                 << index + 1 << "/" << m_milepost[j].cars[k].size()
                                 << "\t" << (*it)->m_pos << "/" << m_milepost[j].mile << endl;
                        }*/
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
                    //auto speed = it->m_speed;
                    it->move(m_period);
                    m_milepost[j].statData[k].speedPeriod += it->m_speed;
                    m_milepost[j].statData[k].carPeriod++;
                }

                // 判断第一辆车是否删除或并入前面
                auto front = m_milepost[j].cars[k].front();
                if (!front)
                {
                    //cout << i << "\t" << j << "\t" << k << "\t" << m_milepost[j].cars[k].size() << endl;
                }
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
                        m_milepost[j - 1].statData[k].carFlow++;
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
                        auto pos = m_milepost[j].mile;
                        if (frontCar)
                        {
                            if (frontCar->m_pos + 20. / 5280. > m_milepost[j].mile)
                            {
                                pos = frontCar->m_pos + 21. / 5280.;
                            }
                        }
                        auto newCar = addCar(m_milepost[j].cars[k],
                                m_milepost[j].idealSpeed, pos, j);
                        m_milepost[j].statData[k].carFlow++;
                        if (frontCar) newCar->m_backCar = frontCar->m_backCar;
                        if (newCar->m_backCar)newCar->m_backCar->m_frontCar = newCar;
                        newCar->m_frontCar = frontCar;
                        if (frontCar) frontCar->m_backCar = newCar;
                    }
                }

                m_milepost[j].statData[k].carTotal += m_milepost[j].cars[k].size();
            }
        }

        if (i == (maxI / 10) && m_ban)
        {
            const double max_distance = 6;
            // 获取第一根车道上最后一辆车
            auto back = m_milepost[m_milepost.size() / 2].cars[0].back();
            auto car = back;
            std::pair<double, double> pair1(now, 0);
            std::vector<std::pair<double, double> > vector1(1, pair1);
            m_banData.push_back(std::make_pair(car, vector1));
            car->m_deltaPos = 0;
            m_banStartNo = car->m_milepostNo;
            m_banStartPos = car->m_pos;
            m_banCarBack = car;
            m_banTime = now;
            double distance = 0;
            double d = 0;
            while (distance <= max_distance)
            {
                auto front = car->m_frontCar;
                if (!front) break;
                if (car->m_milepostNo > front->m_milepostNo)
                {
                    for (int j = front->m_milepostNo; j < car->m_milepostNo; j++)
                    {
                        d += m_milepost[j].mile;
                    }
                }
                d += car->m_pos - front->m_pos;
                if (d < max_distance / 120.)
                {
                    car = front;
                    continue;
                }
                distance += d;
                std::pair<double, double> pair2(now, distance);
                std::vector<std::pair<double, double> > vector2(1, pair2);
                m_banData.push_back(std::make_pair(car, vector2));
                car->m_deltaPos = 0;
                d = 0;
                car = front;
            }
        }
        else if (i > (maxI / 10) && m_ban && i % (int(time / 36)) == 0)
        {
            for (auto &it:m_banData)
            {
                auto lastPos = it.second.back().second;
                it.second.push_back(std::make_pair(now, lastPos + it.first->m_deltaPos));
                it.first->m_deltaPos = 0;
            }
            while (auto front = m_banCarBack->m_backCar)
            {
                double d = 0;
                if (m_banStartNo > front->m_milepostNo)
                {
                    for (int j = front->m_milepostNo; j <= m_banStartNo; j++)
                    {
                        d += m_milepost[j].mile;
                    }
                    d += m_banStartPos - front->m_pos;
                }
                else if (m_banStartNo == front->m_milepostNo && m_banStartPos >= front->m_pos)
                {
                    d = m_banStartPos - front->m_pos;
                }
                if (d > 0)
                {
                    if (now - m_banTime > 0.002 * time)
                    {
                        std::pair<double, double> pair1(now, d);
                        std::vector<std::pair<double, double> > vector1(1, pair1);
                        m_banData.push_front(std::make_pair(front, vector1));
                        front->m_deltaPos = 0;
                        m_banTime = now;
                    }
                }
                else break;
                m_banCarBack = front;
            }

            if (i > (3 * maxI / 10))
            {
                m_ban = false;
            }
        }


        if (i % (maxI / 50) == 0)
        {
            //cout << now << "/" << time << endl;
            cout << "\b->" << flush;
            //printSpeed(600);
        }


    }
}

bool Layout::openFile()
{
    m_file.open(m_outputPath);
    if (!m_file.is_open())
    {
        cerr << "Outpur File Failed to Open!" << endl << m_outputPath;
        return false;
    }
    return true;
}

void Layout::closeFile()
{
    m_file.close();
}

void Layout::printSpeed(double interval)
{
    for (int i = 0; i < m_milepost.size(); i++)
    {
        for (int j = 0; j < m_milepost[i].cars.size(); j++)
        {
            double speedAvg = m_milepost[i].statData[j].carPeriod > 0 ?
                              m_milepost[i].statData[j].speedPeriod / m_milepost[i].statData[j].carPeriod : 0;
            double carFlow = m_milepost[i].statData[j].carFlow;
            double carDensity = m_milepost[i].statData[j].carTotal / interval / m_milepost[i].mile;
            m_file << i << "\t" << j << "\t"
                   << speedAvg << "\t"
                   << carFlow << "\t"
                   << carDensity << "\t"
                   << endl;
        }
    }
    m_file << endl;
    cout << endl;
}

void Layout::printBan()
{
    cout << endl << m_banData.size() << endl;
    m_file << m_banData.size() << endl;
    for (auto &it:m_banData)
    {
        m_file << it.second.size() << endl;
        for (auto &it2:it.second)
        {
            m_file << it2.first << "\t" << it2.second << endl;
        }
    }
}