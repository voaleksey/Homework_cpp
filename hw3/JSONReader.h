#ifndef FSM_JSONREADER
#define FSM_JSONREADER


#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "json.hpp"

#include "State.h"
#include "TransitionFunction.h"

namespace DeeterministicFSM
{
    template <typename TriggerType>
    class JSONReader
    {
    public:
        JSONReader(std::string path)
        {
            // Reading file

            std::ifstream f(path);
            if (!f.is_open()) 
            {
                std::cout << "Error opening file: " << path << std::endl;
                return;
            }

            std::string file_content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            nlohmann::json _data = nlohmann::json::parse(file_content);
            f.close();

            // Rebuild states from JSON 

            _states = {};
            for(int i = 0; i < _data["States"].get<std::vector<bool>>().size(); i++)
            {
                State state(_data["States"].get<std::vector<bool>>()[i]);
                _states.push_back(state);
            }
            
            // Rebuild transitions from JSON

            _transitions = {};
            for(int i = 0; i < _data["Transitions"].size(); i++)
            {
                State from;
                State to;
                TriggerType trigger;
                for(State state : _states)
                {
                    if (state.GetID() == _data["Transitions"][i]["from"].get<int>())
                    {
                        from = state;
                    }
                    if(state.GetID() == _data["Transitions"][i]["to"].get<int>())
                    {
                        to = state;
                    }
                }
                Transition<TriggerType> transition(from, to, _data["Transitions"][i]["trigger"].get<TriggerType>());
                _transitions.push_back(transition);
                
            }
        }

        std::vector<State> GetStates()
        {
            return _states;
        }

        std::vector<Transition<TriggerType>> GetTransitions()
        {
            return _transitions;
        }

    private:
        nlohmann::json _data;
        std::vector<State> _states;
        std::vector<Transition<TriggerType>> _transitions;
    };
}

#endif