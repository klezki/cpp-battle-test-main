#pragma once

#include <cstdint>

#include <Actions/ActionChain.h>


class Unit
{
	const uint32_t _id;
	uint32_t _hp;
	ActionChain _chain;

protected:
	Unit(uint32_t id, uint32_t hp) : _id(id), _hp(hp) {};

	Unit(const Unit&) = default;
	Unit(Unit&&) = default;

public:
	virtual ~Unit() = default;

	uint32_t getId() const { return _id; };
	uint32_t getHp() const { return _hp; };

	void removeHp(uint32_t damage) { _hp = _hp > damage ? _hp - damage : 0u; };

	bool executeActions() { return _chain.execute(); }

	void addAction(std::unique_ptr<Action> action_ptr, uint32_t priority = 0)
	{
		_chain.addAction(std::move(action_ptr), priority);
	}
};