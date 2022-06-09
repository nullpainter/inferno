#ifndef _WIFICONTROL_H_
#define _WIFICONTROL_H_

#include <Arduino.h>
#include <TaskScheduler.h>
#include <WiFiManager.h>

#define SSID "Inferno"

class WifiControl
{
public:
    static void setup(Scheduler *scheduler);
    static void update();
    static bool connected();

private:
    static WiFiManager _wifiManager;
    static Task _wifiConnectTask;
    static bool _connected;
    static void onConnection();
};

#endif