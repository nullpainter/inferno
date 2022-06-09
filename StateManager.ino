#include "LedControl.h"
#include "StateManager.h"

State StateManager::_state = State::Off;
bool StateManager::_initialised = false;

bool StateManager::initialised()
{
    return _initialised;
}

void StateManager::setState(State state)
{

    _initialised = true;
    
    if (_state == state)
    {
        return;
    }

    switch (state)
    {
    case Off:
        Serial.println("Turning LEDs off");
        LedControl::fadeOut();
        break;
    case On:
        Serial.println("Updating state to: On");
        LedControl::fadeIn();

        _state = state;
        break;
    case Monitor:
        Serial.println("Updating state to: Monitor");

        // Briefly display flame effect when monitoring
        LedControl::setIntensity(255);
        LedControl::on();

        LedControl::fadeOut(FADE_DURATION, FADE_DURATION);

        _state = state;
        break;
    }
}

State StateManager::getState()
{
    return _state;
}