#ifndef BATTERYSTATUS_H
#define BATTERYSTATUS_H

#include <fstream>
#include <string>

void logPowerStatus(std::ofstream& logFile);
void parseBatteryReport(const std::string& filePath, std::ofstream& logFile);

#endif // BATTERYSTATUS_H
