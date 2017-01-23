/*************************************************
main.cpp
**************************************************/

#include "mcm.h"
#include "Layout.h"

const double period = 0.1;
double totalTime = 0;
string name = "";

int simulate(const std::vector<Data> &m_data, bool isNormal, bool isASC, string autoPercentage, bool ban)
{
    Layout lay(m_data, isNormal, isASC, atof(autoPercentage.c_str()));
    lay.m_period = period;
    lay.m_outputPath = "output/";
    lay.m_outputPath += name + "_";
    lay.m_outputPath += (isNormal ? "norm_" : "busy_");
    lay.m_outputPath += (isASC ? "asc_" : "desc_");
    lay.m_outputPath += autoPercentage;
    if (ban)lay.m_outputPath += "_ban";
    lay.m_outputPath += ".txt";
    cout << lay.m_outputPath << endl;
    lay.m_ban = ban;
    lay.simulate(totalTime);
    if (!lay.openFile())return -1;
    if(ban)
    {
        lay.printBan();
    }
    else
    {
        lay.printSpeed(totalTime / period);
    }
    lay.closeFile();
    return 0;
}

int main(int argc, char *argv[])
{
    const int MAX_ARG = 4;
    string args[MAX_ARG] = {"5", "0", "3600", ""};
    for (int i = 1; i <= min(argc - 1, MAX_ARG); i++)
    {
        args[i - 1] = argv[i];
    }

    for (int i = 0; i < MAX_ARG; i++)
    {
        cout << args[i] << "\t";
    }
    cout << endl;

    name = args[0];
    string filename = "data/" + name + ".txt";
    totalTime = atof(args[2].c_str());
    bool ban = args[1] != "0";

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
        data.routeId = atoi(name.c_str());
        ss >> data.start >> data.end >> data.traffic
           >> data.type >> data.paneDESC >> data.paneASC;
        m_data.push_back(data);
    }

    cout << m_data.size() << endl;

    std::vector<string> percentVec = {"0.0", "0.1", "0.3", "0.5", "0.7", "0.9", "1.0"};

    for (auto autoPercentage : percentVec)
    {
        if (args[3] == "" || args[3] == "0")
        {
            simulate(m_data, false, false, autoPercentage, ban);
        }
        if (args[3] == "" || args[3] == "1")
        {
            simulate(m_data, false, true, autoPercentage, ban);
        }
        if (args[3] == "" || args[3] == "2")
        {
            simulate(m_data, true, false, autoPercentage, ban);
            simulate(m_data, true, true, autoPercentage, ban);
        }
    }


    return 0;
}