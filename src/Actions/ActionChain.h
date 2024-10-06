#pragma once


#include <memory>
#include <vector>

#include <Actions/Action.h>


class ActionChain
{
	struct ActionInfo
	{
		std::unique_ptr<Action> action_ptr;
		uint32_t priority;
	};

	std::vector<ActionInfo> _chain;
public:
	bool execute()
	{
		for (auto it = _chain.cbegin(); it != _chain.cend(); ++it)
		{
			auto& action = *it->action_ptr;
			bool isExecuted = action.execute();
			if (isExecuted)
			{
				if (action.isConsumed())
					_chain.erase(it); // safe cuz we return right away
				
				return true;
			}
		}
		
		return false;
	}

	void addAction(std::unique_ptr<Action> action_ptr, uint32_t priority = 0)
	{
		if (!priority)
		{
			uint32_t priority = _chain.empty() ? 1 : _chain.back().priority + 1;
			_chain.emplace_back(std::move(action_ptr), priority);
		}
		else
		{
			auto it = std::upper_bound(
				_chain.begin(),
				_chain.end(),
				priority,
				[](const uint32_t& a, const ActionInfo& b) { return a < b.priority; }
			);
			_chain.emplace(it, std::move(action_ptr), priority);
		}

	}
};