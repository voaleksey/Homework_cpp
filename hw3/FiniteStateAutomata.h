#ifndef FSM_FINITESTATEAUTOMATA
#define FSM_FINITESTATEAUTOMATA

#include <vector>

#include "State.h"
#include "TransitionFunction.h"

#include "JSONReader.h"
                
namespace DeeterministicFSM
{
    enum FSAResourceFileType
    {
        JSONFile
    };

    template <typename TriggerType>
    class DeterministicFSM 
    {
    public:
        DeterministicFSM(std::vector<State> states, std::vector<Transition<TriggerType>> transitions, State initialState) :
            _states(states), _transitions(transitions), _currentState(initialState), _initialState(initialState) {}
        DeterministicFSM(std::string path, FSAResourceFileType resourceFileType)
        {
            switch(resourceFileType)
            {
                case(JSONFile):
                {
                    JSONReader<TriggerType> jsonreader(path);

                    _states = jsonreader.GetStates();
                    _transitions = jsonreader.GetTransitions();
                    _initialState = _states[0];
                    _currentState = _states[0];
                    
                    break;
                }
            }
        }

        std::vector<unsigned int> GetTriggeredChainIDs(std::vector<TriggerType> triggers)
        {
            std::vector<unsigned int> chain = {};
            for (int i = 0; i < triggers.size(); i++)
            {
                if (triggerEvent(triggers[i]) == false)
                {
                    return chain;
                }
                chain.push_back(_currentState.GetID());
            }
            return chain;
        }

        bool Run(std::vector<TriggerType> triggers)
        {
            for (int i = 0; i < triggers.size(); i++)
            {
                if (triggerEvent(triggers[i]) == false)
                {
                    return false;
                }
                if (_currentState.GetTerminality())
                {
                    return true;
                }
            }
            return false;
        }

        void Restart()
        {
            _currentState = _initialState;
        }

    private:
        State _initialState;
        State _currentState;
        std::vector<State> _states;
        std::vector<Transition<TriggerType>> _transitions;

        bool triggerEvent(TriggerType value)
        {
            for (auto& transition : _transitions)
            {
                if (transition.GetFromState().GetID() == _currentState.GetID() && transition.GetTrigger() == value)
                {
                    _currentState = transition.GetToState();
                    return true;
                }
            }
            return false;
        }
    };
}

#endif // !FSM_FINITESTATEAUTOMATA