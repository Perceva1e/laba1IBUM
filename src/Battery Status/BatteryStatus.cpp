#include "BatteryStatus.h"
#include <windows.h>
#include <powrprof.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

#pragma comment(lib, "PowrProf.lib")

std::chrono::time_point<std::chrono::steady_clock> chargeStartTime;
bool isCharging = false;

void logPowerStatus(std::ofstream& logFile) {
    SYSTEM_POWER_STATUS sps;
    if (GetSystemPowerStatus(&sps)) {
        logFile << "Power source: " << (sps.ACLineStatus == 1 ? "AC" : "Battery") << std::endl;
        logFile << "Battery status: " << (sps.BatteryFlag == 128 ? "No battery" : "Battery present") << std::endl;
        logFile << "Battery charge level: " << (int)sps.BatteryLifePercent << "%" << std::endl;
        logFile << "Current power saving mode: " << (sps.SystemStatusFlag == 1 ? "Power saving" : "Normal mode") << std::endl;
        logFile << "Battery life time: " << sps.BatteryLifeTime << " seconds" << std::endl;
        logFile << "Battery full life time: " << sps.BatteryFullLifeTime << " seconds" << std::endl;

        if (sps.ACLineStatus == 1) {
            if (!isCharging) {
                chargeStartTime = std::chrono::steady_clock::now();
                isCharging = true;
            }
            auto now = std::chrono::steady_clock::now();
            auto chargeDuration = std::chrono::duration_cast<std::chrono::seconds>(now - chargeStartTime).count();
            logFile << "Charging time: " << chargeDuration << " seconds" << std::endl;
        }
        else {
            isCharging = false;
        }
    }
    else {
        logFile << "Failed to get power status information." << std::endl;
    }
}

void parseBatteryReport(const std::string& filePath, std::ofstream& logFile) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        logFile << "Failed to open file: " << filePath << std::endl;
        return;
    }

    std::string htmlContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::string chemistry, designCapacity, fullChargeCapacity;
    size_t pos = 0;
    bool foundChemistry = false, foundDesignCapacity = false, foundFullChargeCapacity = false;

    while (pos != std::string::npos && !(foundChemistry && foundDesignCapacity && foundFullChargeCapacity)) {
        pos = htmlContent.find("CHEMISTRY", pos);
        if (pos != std::string::npos && !foundChemistry) {
            size_t start = htmlContent.find("<td>", pos) + 4;
            size_t end = htmlContent.find("</td>", start);
            chemistry = htmlContent.substr(start, end - start);
            foundChemistry = true;
            pos = end;
        }

        pos = htmlContent.find("DESIGN CAPACITY", pos);
        if (pos != std::string::npos && !foundDesignCapacity) {
            size_t start = htmlContent.find("<td>", pos) + 4;
            size_t end = htmlContent.find("</td>", start);
            designCapacity = htmlContent.substr(start, end - start);
            foundDesignCapacity = true;
            pos = end;
        }

        pos = htmlContent.find("FULL CHARGE CAPACITY", pos);
        if (pos != std::string::npos && !foundFullChargeCapacity) {
            size_t start = htmlContent.find("<td>", pos) + 4;
            size_t end = htmlContent.find("</td>", start);
            fullChargeCapacity = htmlContent.substr(start, end - start);
            foundFullChargeCapacity = true;
            pos = end;
        }
    }

    logFile << "CHEMISTRY: " << chemistry << std::endl;
    logFile << "DESIGN CAPACITY: " << designCapacity << std::endl;
    logFile << "FULL CHARGE CAPACITY: " << fullChargeCapacity << std::endl;
}
