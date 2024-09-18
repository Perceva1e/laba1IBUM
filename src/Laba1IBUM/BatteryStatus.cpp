#include "BatteryStatus.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <powrprof.h>

#pragma comment(lib, "PowrProf.lib")
const std::string logFilePath = "C:\\Users\\denis\\source\\repos\\laba1IBUMSTATUS\\battery_log.txt";

void readBatteryData(std::string& powerSource, std::string& batteryStatus, int& batteryChargeLevel, std::string& powerSavingMode, long long& batteryLifeTime, long long& batteryFullLifeTime, int& chargingTime, std::string& chemistry, std::string& designCapacity, std::string& fullChargeCapacity) {
    std::ifstream logFile(logFilePath);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(logFile, line)) {
        if (line.find("Power source:") != std::string::npos) {
            powerSource = line.substr(line.find(":") + 2);
        }
        else if (line.find("Battery status:") != std::string::npos) {
            batteryStatus = line.substr(line.find(":") + 2);
        }
        else if (line.find("Battery charge level:") != std::string::npos) {
            try {
                batteryChargeLevel = std::stoi(line.substr(line.find(":") + 2));
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid battery charge level." << std::endl;
            }
        }
        else if (line.find("Current power saving mode:") != std::string::npos) {
            powerSavingMode = line.substr(line.find(":") + 2);
        }
        else if (line.find("Battery life time:") != std::string::npos) {
            try {
                batteryLifeTime = std::stoll(line.substr(line.find(":") + 2));
                if (batteryLifeTime == -1) {
                    batteryLifeTime = 0;
                }
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid battery life time." << std::endl;
            }
        }
        else if (line.find("Battery full life time:") != std::string::npos) {
            try {
                batteryFullLifeTime = std::stoll(line.substr(line.find(":") + 2));
                if (batteryFullLifeTime == -1) {
                    batteryFullLifeTime = 0;
                }
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid battery full life time." << std::endl;
            }
        }
        else if (line.find("Charging time:") != std::string::npos) {
            try {
                chargingTime = std::stoi(line.substr(line.find(":") + 2));
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid charging time." << std::endl;
            }
        }
        else if (line.find("CHEMISTRY:") != std::string::npos) {
            chemistry = line.substr(line.find(":") + 2);
        }
        else if (line.find("DESIGN CAPACITY:") != std::string::npos) {
            designCapacity = line.substr(line.find(":") + 2);
        }
        else if (line.find("FULL CHARGE CAPACITY:") != std::string::npos) {
            fullChargeCapacity = line.substr(line.find(":") + 2);
        }
    }

    logFile.close();
}

void showPowerStatus(HWND hwnd) {
    std::string powerSource, batteryStatus, powerSavingMode, chemistry, designCapacity, fullChargeCapacity;
    int batteryChargeLevel = 0, chargingTime = 0;
    long long batteryLifeTime = 0, batteryFullLifeTime = 0;

    readBatteryData(powerSource, batteryStatus, batteryChargeLevel, powerSavingMode, batteryLifeTime, batteryFullLifeTime, chargingTime, chemistry, designCapacity, fullChargeCapacity);
    int designCapacityValue = std::stoi(designCapacity);
    int fullChargeCapacityValue = std::stoi(fullChargeCapacity);
    double capacityRatio = static_cast<double>(fullChargeCapacityValue) / designCapacityValue;
    std::string status = "Power source: " + powerSource + "\n" +
        "Battery status: " + batteryStatus + "\n" +
        "Battery charge level: " + std::to_string(batteryChargeLevel) + "%\n" +
        "Current power saving mode: " + powerSavingMode + "\n" +
        "Battery life time: " + std::to_string(batteryLifeTime) + " seconds\n" +
        "Battery full life time: " + std::to_string(batteryFullLifeTime) + " seconds\n" +
        "Charging time: " + std::to_string(chargingTime) + " seconds\n" +
        "CHEMISTRY: " + chemistry + "\n" +
        "DESIGN CAPACITY: " + designCapacity + "\n" +
        "FULL CHARGE CAPACITY: " + fullChargeCapacity + "\n" +
        "Health Battery: " + std::to_string(capacityRatio);

    MessageBox(hwnd, std::wstring(status.begin(), status.end()).c_str(), L"Power Status", MB_OK);
    std::cout << status << std::endl;
}

void enterSleepMode() {
    SetSuspendState(FALSE, FALSE, FALSE);
}

void enterHibernateMode() {
    SetSuspendState(TRUE, FALSE, FALSE);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        CreateWindow(L"BUTTON", L"Show Power Status", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 50, 200, 30, hwnd, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        CreateWindow(L"BUTTON", L"Sleep", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 100, 200, 30, hwnd, (HMENU)2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        CreateWindow(L"BUTTON", L"Hibernate", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 150, 200, 30, hwnd, (HMENU)3, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 1) {
            showPowerStatus(hwnd);
        }
        else if (LOWORD(wParam) == 2) {
            enterSleepMode();
        }
        else if (LOWORD(wParam) == 3) {
            enterHibernateMode();
        }
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    default: {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
    return 0;
}
