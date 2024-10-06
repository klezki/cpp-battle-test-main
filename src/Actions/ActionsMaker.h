#pragma once

#include <memory>

#include <Simulation.h>
#include <Actions/Actions.h>
#include <Actions/Strategies/AttackStrategies.h>
#include <Actions/Strategies/TargetStrategies.h>


class ActionsMaker
{
protected:
	Simulation& _sim;
public:
	ActionsMaker(Simulation& sim) : _sim(sim) {};

	std::unique_ptr<Attack> makeMeleeAttack(const MeleeUnit& unit) const
	{
		return std::make_unique<Attack>(
			_sim,
			std::make_unique<MeleeAttack>(_sim, unit),
			std::make_unique<MeleeTarget>(_sim, unit)
		);
	}

	std::unique_ptr<Attack> makeRangedAttack(const RangedUnit& unit) const
	{
		return std::make_unique<Attack>(
			_sim,
			std::make_unique<RangedAttack>(_sim, unit),
			std::make_unique<RangedTarget>(_sim, unit)
		);
	}

	std::unique_ptr<SimpleMove> makeMove(const Unit& unit, Position target) const
	{
		return std::make_unique<SimpleMove>(_sim, unit, target);
	}
};