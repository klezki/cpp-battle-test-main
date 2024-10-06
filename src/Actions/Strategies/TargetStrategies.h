#pragma once

#include <Simulation.h>
#include <Units/Units.h>
#include <Actions/Strategies/TargetStrategy.h>


class MeleeTarget : public TargetStrategy
{
public:
	using TargetStrategy::TargetStrategy;

	virtual std::optional<Position> getTarget() const override
	{
		Position owner_pos = _sim.getPos(_owner.getId());
		return _sim.getFirstUnit(1, owner_pos);
	};
};


class RangedTarget : public TargetStrategy
{
	constexpr static int MIN_RANGE = 2;

public:
	RangedTarget(const Simulation& sim, const RangedUnit& owner) : TargetStrategy(sim, owner) {};

	virtual std::optional<Position> getTarget() const override
	{
		const RangedUnit& owner = static_cast<const RangedUnit&>(_owner);

		for (uint32_t radius = MIN_RANGE; radius <= owner.getRange(); radius++)
		{
			Position owner_pos = _sim.getPos(_owner.getId());
			auto op = _sim.getFirstUnit(radius, owner_pos);
			if (op)
				return op;
		}

		return {};
	};
};


//class RavenRangedStrategy : public TargetStrategy
//{
//public:
//	RavenRangedStrategy(const Simulation& sim, const RangedUnit owner) : TargetStrategy(sim, owner) {};
//
//	virtual std::optional<Position> getTarget() const override
//	{
//		Position owner_pos = _sim.getPos(_owner.getId());
//		return _sim.getFirstUnitWithType(1, owner_pos, unitTypes::RAVEN);
//	};
//};