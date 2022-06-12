#include <Arduino.h>
#include "LedControl.h"
#include "StateManager.h"

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
    case Off:
        // Note that the state is deliberately not changed to off as this is intended
        // as a mechanism to silence alerts
        Serial.println("[StateManager] Turning LEDs off");

        if (!immediate)
        {
            LedControl::fadeOut();
        }
        break;
    case On:
        Serial.println("[StateManager] Updating state to on");

        if (!immediate)
        {
            LedControl::fadeIn();
        }

        _state = state;
        break;
    case Monitor:
        Serial.println("Updating state to monitor");

        if (!immediate)
        {
            // Briefly display flame effect when monitoring
            LedControl::setIntensity(MAX_INTENSITY);
            LedControl::fadeOut(FADE_DURATION, FADE_DURATION * 5);
        }

        _state = state;
        break;
    }
}

State StateManager::getState()
{
    return _state;
}