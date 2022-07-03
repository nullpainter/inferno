#include <LittleFS.h>

#include "Webserver.h"
#include "StateManager.h"
#include "Slackbot.h"
#include "WifiControl.h"
#include "LedControl.h"
#include "Inferno.h"

Webserver webserver;
StateManager stateManager;

void setup()
{
  Serial.begin(115200);

  WifiControl::setup();
  LedControl::setup();

  LittleFS.begin();
  webserver.setup();
}

bool _wifiConnected;

void loop()
{
  WifiControl::update();

  if (!WifiControl::connected())
  {
    LedControl::setMode(FlameMode::Offline);
    _wifiConnected = false;
  }
  else
  {
    // Reset to flame mode after WiFi is connected
    if (!_wifiConnected)
    {
      LedControl::setMode(FlameMode::Flame);
      _wifiConnected = true;
    }

    // Set default state to monitoring
    if (!StateManager::initialised())
    {
      StateManager::setState(State::Monitor);
    }

    webserver.handleClient();
    Slackbot::update();
  }

  LedControl::update();
}