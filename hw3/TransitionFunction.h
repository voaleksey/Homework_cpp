#ifndef FSM_TRANSITIONFUNCTION
#define FSM_TRANSITIONFUNCTION

#include "State.h"

namespace DeeterministicFSM
{
    template <typename TriggerType>
    class Transition 
    {
    public:
        Transition() {}
        Transition(State fromState, State toState, TriggerType trigger) :
            _fromState(fromState), _toState(toState), _trigger(trigger) {}

        Transition(const Transition<TriggerType>& other)
        {
            this->_fromState = other._fromState;
            this->_toState = other._toState;
            this->_trigger = other._trigger;
        }

        ~Transition() {}

        Transition& operator=(const Transition& other)
        {
            if (this != &other)
            {
                _fromState = other._fromState;
                _toState = other._toState;
                _trigger = other._trigger;
            }
            return *this;
        }

        State GetFromState()
        {
            return _fromState;
        }

        State GetToState()
        {
            return _toState;
        }

        TriggerType GetTrigger() const
        {
            return _trigger;
        }

    private:
        State _fromState;
        State _toState;
        TriggerType _trigger;
    };

}

#endif // !FSM_TRANSITIONFUNCTION

