#ifndef FSM_STATE
#define FSM_STATE

#include <string>

namespace DeeterministicFSM
{
	class State
	{
	public:
		State() {}
		State(bool isTerminal) :
			_id(_LatestID), _isTerminal(isTerminal)
		{
			_LatestID++;
		}

		State(const State& other)
		{
				this->_id = other._id;
				this->_isTerminal = other._isTerminal;
		}
		~State() {}

		State& operator=(const State& other)
		{
			if (this != &other)
			{
				this->_id = other._id;
				this->_isTerminal = other._isTerminal;
			}
			return *this;
		}

		unsigned int GetID() const
		{
			return _id;
		}


		bool GetTerminality() const
		{
			return _isTerminal;
		}

	private:
		unsigned int _id; // Represents ID of this State
		bool _isTerminal; // Determines if class is in a set of final states
		static unsigned int _LatestID;
	};
	unsigned int DeeterministicFSM::State::_LatestID = 0;
}

#endif // FSM_STATE

