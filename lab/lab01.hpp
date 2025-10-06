#pragma once
#include <string>

class batteryMonitor
{
public:
    batteryMonitor();
    std::string getStatus();
    int getCharge();
    std::string getPowerMode();
    int hibernate();
    int sleep();
    std::string getBatteryInfo();
    int getTimeLeft();
    std::string isEco();
};
