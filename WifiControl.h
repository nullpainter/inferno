#ifndef _WIFICONTROL_H_
#define _WIFICONTROL_H_

#include <WiFiManager.h>

#define SSID "Inferno"

class WifiControl
{
public:
    static void setup();
    static void update();
    static bool connected();

private:
    static WiFiManager _wifiManager;
    static bool _connected;
};

#endif