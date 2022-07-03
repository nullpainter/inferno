#include <Arduino.h>
#include <FastLED.h>
#include "StateManager.h"
#include "LedControl.h"

State StateManager::_state = State::Off;
bool StateManager::_initialised = false;

bool StateManager::initialised()
{
    return _initialised;
}

void StateManager::setState(State state, bool immediate)
{

    _initialised = true;

    if (_state == state)
    {
        return;
    }

    switch (state)
    {
    case State::Off:

        // Note that the state is deliberately not changed to off as this is intended
        // as a mechanism to silence alerts
        Serial.println("[StateManager] Turning LEDs off");

        if (!immediate)
        {
            LedControl::fadeOut();
        }
        break;
    case State::On:
        Serial.println("[StateManager] Updating state to on");

        if (!immediate)
        {
            LedControl::fadeIn();
        }

        _state = state;
        break;
    case State::Monitor:
        Serial.println("[StateManager] Updating state to monitor");

        if (!immediate)
        {
            // Briefly display flame effect when monitoring
            LedControl::on();
            LedControl::fadeOut(FADE_DURATION * 10);
        }

        _state = state;
        break;
    }
}

State StateManager::getState()
{
    return _state;
}