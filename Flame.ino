#include <TaskScheduler.h>
#include <LittleFS.h>

#include "Led.h"
#include "LedControl.h"
#include "Webserver.h"
#include "StateManager.h"
#include "Slackbot.h"
#include "WifiControl.h"
#include "Flame.h"

Led leds[] = {Led(D0), Led(D1), Led(D4), Led(D8)};

Scheduler scheduler;
Webserver webserver;
StateManager stateManager;

Task fireTask(TASK_IMMEDIATE, TASK_FOREVER, &fireCallback);

void setup()
{
  Serial.begin(115200);

  WifiControl::setup(&scheduler);

  scheduler.addTask(fireTask);
  fireTask.enable();

  LittleFS.begin();
  webserver.setup();
}

void loop()
{
  if (WifiControl::connected())
  {
    // Set default state to monitoring
    if (!StateManager::initialised())
    {
      StateManager::setState(Monitor);
    }

    // Prioritise LED update
    if (ticks() % 10 == 0)
    {
      webserver.handleClient();
      scheduler.execute();
      Slackbot::update();
    }

    LedControl::update();
  }

  WifiControl::update();
}

void fireCallback()
{
  int interval;
  Led led = leds[random(0, NUM_LEDS)];

  // Turn on and off LEDs on each job execution
  if (fireTask.getRunCounter() & 1)
  {
    led.on();
    interval = random(MIN_DUTY_ON, MAX_DUTY_ON);
  }
  else
  {
    led.off();
    interval = random(MIN_DUTY_OFF, MAX_DUTY_OFF);
  }

  fireTask.setInterval(interval);
}