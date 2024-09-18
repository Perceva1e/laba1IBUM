#include "BatteryStatus.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

int main() {
    const std::string batteryReportPath = "C:\\Users\\denis\\source\\repos\\laba1IBUMSTATUS\\battery-report.html";

    while (true) {
        std::ofstream logFile("battery_log.txt", std::ios_base::trunc);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file." << std::endl;
            return 1;
        }
        logPowerStatus(logFile);
        parseBatteryReport(batteryReportPath, logFile);
        logFile << "----------------------------------------" << std::endl;
        logFile.close();
        Sleep(6000);
    }

    return 0;
}
