#ifndef _WIFICONTROL_H_
#define _WIFICONTROL_H_

#include <WiFiManager.h>

#define SSID "Inferno"

class WifiControl
{
public:
    /**
     * @brief initialises WiFi
     */
    static void setup();

    /**
     * @brief 
     */
    static void update();

    /**
     * @return true if Inferno has connected to a WiFi network
     */
    static bool connected();

private:
    static WiFiManager _wifiManager;
    static bool _connected;
};

#endif