Battery Status Monitoring Applications
Overview
Этот репозиторий содержит два приложения для мониторинга состояния батареи ноутбука. Оба приложения разработаны на языке C++ и используют Windows API для взаимодействия с батареей. Они предоставляют информацию о состоянии питания, уровне заряда, режиме энергосбережения и химии батареи.

Applications
1. Battery Status Logger
Описание: Приложение для записи текущего состояния батареи в лог-файл.

Функциональность:

Получение информации о состоянии батареи, такой как источник питания, уровень заряда, оставшееся время работы и режим энергосбережения.
Запись полученной информации в лог-файл.
Автоматическое обновление информации о батарее в реальном времени.
Использование:

Клонируйте репозиторий:
git clone https://github.com/yourusername/battery-status-monitoring.git
cd battery-status-monitoring

Скомпилируйте и запустите приложение:
g++ -o BatteryStatusLogger BatteryStatusLogger.cpp -lpowrprof
./BatteryStatusLogger

2. Battery Status GUI
Описание: Приложение с графическим интерфейсом для отображения текущего состояния батареи и управления состоянием ноутбука.

Функциональность:

Отображение информации о состоянии батареи в графическом интерфейсе.
Управление состоянием ноутбука (спящий режим и гибернация).
Использование:

Клонируйте репозиторий:
git clone https://github.com/yourusername/battery-status-monitoring.git
cd battery-status-monitoring

Скомпилируйте и запустите приложение:
g++ -o BatteryStatusGUI BatteryStatusGUI.cpp -lgdi32 -lpowrprof
./BatteryStatusGUI

Requirements
Windows OS
Компилятор C++ (например, g++)
Windows API
