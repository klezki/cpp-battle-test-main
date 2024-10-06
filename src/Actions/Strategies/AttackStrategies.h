#pragma once

#include <cstdint>

#include <Simulation.h>
#include <Units/Units.h>
#include <Actions/Strategies/AttackStrategy.h>


class MeleeAttack : public AttackStrategy
{
public:
	MeleeAttack(Simulation& sim, const MeleeUnit& owner) : AttackStrategy(sim, owner) {};

	virtual void attack(Position pos) const override
	{
		const MeleeUnit& owner = static_cast<const MeleeUnit&>(_owner);
		auto damage = owner.getStrength();
		uint32_t targetId = _sim.getUnitId(pos);
		_sim.onAttack(owner.getId(), targetId, damage);
	};
};


class RangedAttack : public AttackStrategy
{
public:
	RangedAttack(Simulation& sim, const RangedUnit& owner) : AttackStrategy(sim, owner) {};

	virtual void attack(Position pos) const override
	{
		const RangedUnit& owner = static_cast<const RangedUnit&>(_owner);
		auto damage = owner.getAgility();
		uint32_t targetId = _sim.getUnitId(pos);
		_sim.onAttack(owner.getId(), targetId, damage);
	};
};

//class FireballAttack : public AttackStrategy
//{
//public:
//	RangedAttack(Simulation& sim, const CasterUnit& owner) : AttackStrategy(sim, owner) {};
//
//	virtual void attack(Position pos) const override
//	{
//		const CasterUnit& owner = static_cast<const CasterUnit&>(_owner);
//		auto damage = owner.getIntellegence();
//		for (position in <positions around pos>)
//			uint32_t targetId = _sim.getUnitId(pos);
//			_sim.onAttack(owner.getId(), targetId, damage);
//	};
//};