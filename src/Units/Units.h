#pragma once

#include <cstdint>

#include <Units/Unit.h>

class MeleeUnit : public Unit
{
	uint32_t _strength;
public:
	MeleeUnit(
		uint32_t id,
		uint32_t hp,
		uint32_t strength
	) : Unit(id, hp), _strength(strength) {};

	uint32_t getStrength() const { return _strength; };
};


class RangedUnit : public MeleeUnit
{
	uint32_t _agility;
	uint32_t _range;
public:

	RangedUnit(
		uint32_t id,
		uint32_t hp,
		uint32_t strength,
		uint32_t agility,
		uint32_t range
	) : MeleeUnit(id, hp, strength), _agility(agility), _range(range) {};

	uint32_t getAgility() const { return _agility; };
	uint32_t getRange() const { return _range; };
};
