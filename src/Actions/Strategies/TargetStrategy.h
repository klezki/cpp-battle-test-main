#pragma once

#include <optional>

#include <Simulation.h>
#include <Units/Units.h>


class TargetStrategy
{
protected:
	const Unit& _owner;
	const Simulation& _sim;
public:
	TargetStrategy(const Simulation& sim, const Unit& owner) : _owner(owner), _sim(sim) {};

	virtual ~TargetStrategy() = default;

	virtual std::optional<Position> getTarget() const = 0;
};