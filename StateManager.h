
#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_

enum State
{
    On,
    Off,
    Monitor
};

class StateManager
{
public:
    /**
     * @brief Sets the operating state of Inferno
     * 
     * @param state state to set
     * @param immediate whether to immediately change the state or to perform state-related animation   
     */
    static void setState(State state, bool immediate = false);
    static State getState();
    static bool initialised();

private:
    static State _state;
    static bool _initialised;
};

#endif