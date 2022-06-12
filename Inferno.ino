#include <TaskScheduler.h>
#include <LittleFS.h>

#include "Led.h"
#include "LedControl.h"
#include "Webserver.h"
#include "StateManager.h"
#include "Slackbot.h"
#include "WifiControl.h"
#include "Inferno.h"

Led leds[] = {Led(D1), Led(D3), Led(D5), Led(D7)};
Led *redLed = &leds[1];

Scheduler scheduler;
Webserver webserver;
StateManager stateManager;

void fireCallback();
void wifiConnectCallback();

Task fireTask(TASK_IMMEDIATE, TASK_FOREVER, &fireCallback);
Task wifiConnectTask(BLINK_DURATION, TASK_FOREVER, &wifiConnectCallback);

void setup()
{
  Serial.begin(115200);

  WifiControl::setup();

  scheduler.addTask(fireTask);
  scheduler.addTask(wifiConnectTask);

  LittleFS.begin();
  webserver.setup();
}

void loop()
{
  WifiControl::update();
  LedControl::update();
  scheduler.execute();

  if (!WifiControl::connected())
  {
    // Enable WiFi connect animation
    if (!wifiConnectTask.isEnabled())
    {
      LedControl::setIntensity(0);
      wifiConnectTask.enable();
    }
  }
  else
  {
    // Disable WiFi connect animation
    if (wifiConnectTask.isEnabled())
    {
      wifiConnectTask.disable();
    }

    // Enable fire animation
    if (!fireTask.isEnabled())
    {
      fireTask.enable();
    }

    // Set default state to monitoring
    if (!StateManager::initialised())
    {
      StateManager::setState(Monitor);
    }

    webserver.handleClient();
    Slackbot::update();
  }
}

void wifiConnectCallback()
{
  redLed->toggle();
  redLed->update();
}

void fireCallback()
{
  Led *led = &leds[random(NUM_LEDS)];

  int intensity = random(MIN_INTENSITY, MAX_INTENSITY + 1);
  int interval = random(MIN_LED_ON, MAX_LED_ON + 1);

  led->setIntensity(intensity);
  fireTask.setInterval(interval);
}