#ifndef BATTERYSTATUS_H
#define BATTERYSTATUS_H

#include <windows.h>
#include <string>

void readBatteryData(std::string& powerSource, std::string& batteryStatus, int& batteryChargeLevel, std::string& powerSavingMode, long long& batteryLifeTime, long long& batteryFullLifeTime, int& chargingTime, std::string& chemistry, std::string& designCapacity, std::string& fullChargeCapacity);
void showPowerStatus(HWND hwnd);
void enterSleepMode();
void enterHibernateMode();
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // BATTERYSTATUS_H
