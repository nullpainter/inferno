#include <ESP8266WiFi.h>
#include <Blinkenlight.h>
#include <ESP8266WiFi.h>

#include "WifiControl.h"
#include "Flame.h"
#include "LedControl.h"

Blinkenlight red(RED);

Task WifiControl::_wifiConnectTask = Task();
WiFiManager WifiControl::_wifiManager;
bool WifiControl::_connected = false;

void WifiControl::setup(Scheduler *scheduler)
{
    LedControl::off();

    // TODO replace this with bespoke blinky
    // TODO get this LED stuff out of here
    const SpeedSetting speed = {
        .on_ms = 500,
        .off_ms = 500,
        .pause_ms = 400,
        .ending_ms = 800,
    };

    red.blink(speed);
    _wifiConnectTask.enable();

    // _wifiManager.erase();
    _wifiManager.setConfigPortalBlocking(false);

    _connected = _wifiManager.autoConnect(SSID);

    if (_connected)
    {
        onConnection();
    }
}

bool WifiControl::connected()
{
    return _connected;
}

void WifiControl::onConnection()
{
    Serial.println("Stopping connection animation");

    _wifiConnectTask.disable();
    red.off();
}

void WifiControl::update()
{
    if (!_connected)
    {
        red.update();
    }

    if (_wifiManager.process())
    {
        onConnection();
    }

    _connected = WiFi.status() == WL_CONNECTED;
}
