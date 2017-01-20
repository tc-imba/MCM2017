#include "mcm.h"

int main()
{
    auto test1 = getIdealSpeedDistance(65000 * 0.04, 3);
    auto test2 = getIdealSpeedDistance(65000 * 0.08, 3);
    cout << test1 << endl << test2 << endl;
    return 0;
}