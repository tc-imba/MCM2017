#include "mcm.h"
#include "Layout.h"

int main(int argc, char *argv[])
{
    const int MAX_ARG = 6;
    string args[MAX_ARG] = {"5", "0", "0", "0", "0.1", "3600"};
    for (int i = 1; i <= min(argc, MAX_ARG); i++)
    {
        args[i - 1] = argv[i];
    }

    string filename = "data/" + args[0] + ".txt";
    bool isNormal = args[1] == "1";
    bool isASC = args[2] == "1";
    double autoPercentage = atof(args[3].c_str());
    double period = atof(args[4].c_str());
    double totalTime = atof(args[5].c_str());

    srand((unsigned) time(NULL));

    ifstream in(filename);

    if (!in.is_open())
    {
        cerr << "File not found!" << endl;
        return -1;
    }

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
    Layout lay(m_data, isNormal, isASC, autoPercentage);
    lay.m_period = period;
    lay.m_outputPath = "output/" + args[0] + "/";
    lay.simulate(totalTime);
    return 0;
}