//
// Created by liu on 2017/1/20.
//

#ifndef MCM2017_MCM_H
#define MCM2017_MCM_H

using namespace std;

// STD LIBS
#include <stdlib.h>
#include <math.h>

// IO Stream
#include <iostream>
#include <fstream>
#include <sstream>

// STL
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <algorithm>

template<typename T1, typename T2>
ostream &operator<<(ostream &os, const std::pair<T1, T2> T)
{
    return os << "pair(" << T.first << "," << T.second << ")";
};


/**
 *
 * @param hourCount The number of cars going through per hour
 * @param paneNum The number of the panes on the roud
 * @return pair<double v, double s>
 * v is the average speed in mile/h
 * s is the distance between two heads of the cars
 */
pair<double, double> getRealitySpeedDistance(double hourCount, int paneNum)
{
    const double a = 81576.5927;
    const double carLength = 16. / 5280.;
    const double maxV = 60.;

    double A = hourCount / (2 * a);
    double B = hourCount / 3600. - paneNum;
    double C = carLength * hourCount;
    double Delta = B * B - 2 * A * C;
    if (Delta <= 0) return pair<double, double>(-1, -1);
    double v = (-B + sqrt(Delta)) / (2 * A);
    v = min(v, maxV);
    double s = v * v / (2 * a) + v / 3600. + carLength;
    return pair<double, double>(v, s);
}


#endif //MCM2017_MCM_H
