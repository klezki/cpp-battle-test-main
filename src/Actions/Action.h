#pragma once

#include <stdexcept>


class Simulation;


class Action
{
protected:
	Simulation& _sim;
	bool _isConsumed = false;
public:
	Action(Simulation& sim) : _sim(sim) {};
	virtual ~Action() = default;

	bool execute()
	{
		if (isConsumed())
			throw std::runtime_error("trying to execute a consumed action");

		bool isExecuted = executeImpl();
		
		if (isExecuted)
			updateConsume();
		
		return isExecuted;
	}
	
	bool isConsumed() const { return _isConsumed; };
	
	virtual bool executeImpl() const = 0;
	virtual void updateConsume() = 0;
};