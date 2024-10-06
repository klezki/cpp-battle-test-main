#pragma once

#include <memory>

#include <Units/Units.h>
#include <Actions/ActionsMaker.h>
#include <Simulation.h>


class UnitsMaker
{
protected:
	const Simulation& _sim;
	const ActionsMaker& _actionsMaker;
public:
	UnitsMaker(const Simulation& sim, const ActionsMaker& actionsMaker) : _sim(sim), _actionsMaker(actionsMaker) {};

	std::unique_ptr<Unit> makeWarror(
		uint32_t unitId,
		uint32_t hp,
		uint32_t strength
	)
	{
		auto warrior = std::make_unique<MeleeUnit>(unitId, hp, strength);

		warrior->addAction(_actionsMaker.makeMeleeAttack(*warrior));

		return warrior;
	}

	std::unique_ptr<Unit> makeArcher(
		uint32_t unitId,
		uint32_t hp,
		uint32_t strength,
		uint32_t agility,
		uint32_t range
	)
	{
		auto arh = std::make_unique<RangedUnit>(unitId, hp, strength, agility, range);

		arh->addAction(_actionsMaker.makeMeleeAttack(*arh));

		//arh->addAction(_actionsMaker.makeRavenAttack(*arh));

		arh->addAction(_actionsMaker.makeRangedAttack(*arh));

		return arh;
	}
};