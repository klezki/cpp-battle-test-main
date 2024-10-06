#pragma once

#include <Simulation.h>
#include <Units/Units.h>


class AttackStrategy
{
protected:
	const Unit& _owner;
	Simulation& _sim;
public:
	AttackStrategy(Simulation& sim, const Unit& owner) : _owner(owner), _sim(sim) {};

	virtual ~AttackStrategy() = default;

	virtual void attack(Position pos) const = 0;
};