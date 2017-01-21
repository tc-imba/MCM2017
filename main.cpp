#include "mcm.h"
#include "Car.h"
#include "HumanCar.h"
#include "Layout.h"


const double period = 0.1;

/*void tick(std::list<Car *> *carQueue)
{
    for (auto it = carQueue->begin(); it != carQueue->end(); ++it)
    {
        (*it)->m_pos -= (*it)->m_speed * period / 3600.;
        if ((*it)->m_pos <= 0 && it != carQueue->begin())
        {
            auto it2 = it;
            carQueue->erase(--it2);
        }
    }
}

void simulate(double hourCount, int paneNum, double milepost)
{
    auto speedDistance = getIdealSpeedDistance(hourCount, paneNum);
    cout << speedDistance << endl;

    auto speed = speedDistance.first, distance = speedDistance.second;
    auto idealCount = milepost / distance;
    auto idealPeriod = distance / speed * 3600; // in seconds
    cout << idealCount << endl << idealPeriod << endl;

    std::list<Car *> *carQueue[paneNum];

    for (int i = 0; i < paneNum; i++)
    {
        carQueue[i] = new std::list<Car *>();
        for (int j = 1; j <= idealCount; j++)
        {
            addCar(carQueue[i], speed, j * distance);
        }
    }

    double lastEnterTime = ((int) idealCount - idealCount) * idealPeriod;

    for (int i = 0; i < 7200; i++)
    {
        double now = i * period;
        bool addCarFlag = false;
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
        }
    }
    cout << carQueue[0]->size() << endl;
}


void process(const Data &data)
{
    simulate(data.traffic * 0.02, data.paneDESC, data.end - data.start);
    simulate(data.traffic * 0.02, data.paneASC, data.end - data.start);
    simulate(data.traffic * 0.04, data.paneDESC, data.end - data.start);
    simulate(data.traffic * 0.04, data.paneASC, data.end - data.start);
}

void layout(const vector <Data> &m_data, std::pair<bool, bool> condition)
{
    std::vector<std::pair<double, double> > v_d_vec;
    for (int i = 0; i < 10; i++)
    {
        int paneNum = condition.second ? m_data[i].paneASC : m_data[i].paneDESC;
        double traffic = m_data[i].traffic * condition.first ? 0.02 : 0.04;
        auto speedDistance = getIdealSpeedDistance(m_data[i].traffic * 0.02, paneNum);
        v_d_vec.push_back(speedDistance);
        std::list<Car *> carQueue[paneNum];

    }
}*/

int main()
{
    ifstream in("data/5.txt");
    std::vector<Data> m_data;
    while (!in.eof())
    {
        string str;
        getline(in, str);
        istringstream ss(str);
        if (ss.eof())continue;
        Data data;
        data.routeId = 5;
        ss >> data.start >> data.end >> data.traffic
           >> data.type >> data.paneDESC >> data.paneASC;
        m_data.push_back(data);
    }
    Layout lay(m_data, false, true);
    return 0;
}