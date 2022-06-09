
#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_

#include "Arduino.h"

enum State
{
    On,
    Off,
    Monitor
};

class StateManager
{
public:
    static void setState(State state);
    static State getState();
    static bool initialised();

private:
    static State _state;
    static bool _initialised;
};

#endif