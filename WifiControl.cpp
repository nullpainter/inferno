#include <ESP8266WiFi.h>
#include "WifiControl.h"

WiFiManager WifiControl::_wifiManager;
bool WifiControl::_connected;

void WifiControl::setup()
{
    // Uncomment to force WiFi reconfiguration
    // _wifiManager.erase();

    _wifiManager.setConfigPortalBlocking(false);
    _connected = _wifiManager.autoConnect(SSID);
}

bool WifiControl::connected()
{
    return _connected;
}

void WifiControl::update()
{
    _wifiManager.process();
    _connected = WiFi.status() == WL_CONNECTED;
}
