#pragma once

#include <Simulation.h>
#include <Actions/Strategies/AttackStrategy.h>
#include <Actions/Strategies/TargetStrategy.h>
#include <Units/Units.h>
#include <Actions/Action.h>


class Attack : public Action
{
	std::unique_ptr<const AttackStrategy> _as;
	std::unique_ptr<const TargetStrategy> _ts;
public:
	Attack(
		Simulation& sim, 
		std::unique_ptr<const AttackStrategy> as, 
		std::unique_ptr<const TargetStrategy> ts) 
		: Action(sim), _as(std::move(as)), _ts(std::move(ts))
	{};

	virtual bool executeImpl() const override
	{
		auto op = _ts->getTarget();
		if (!op)
			return false;
		_as->attack(*op);
		return true;
	}

	virtual void updateConsume() override {}
};

//class Fireball : public Attack
//{
//protected:
//	int charges;
//public:
//
//
//	virtual bool updateConsume() const override
//	{
//		--charges;
//		_isConsumed = charges <= 0;
//	}
//};


class DependantAction : public Action
{
protected:
	const Unit& _owner;
public:
	DependantAction(Simulation& sim, const Unit& owner) : Action(sim), _owner(owner) {};
};



class SimpleMove : public DependantAction
{
	const Position _targetPos;
public:
	SimpleMove(Simulation& sim, const Unit& owner, const Position pos) : DependantAction(sim, owner), _targetPos(pos) {};

	virtual bool executeImpl() const override
	{
		// assuming no obsticales, no other units (otherwise we should have attacked them), etc

		Position ownPos = _sim.getPos(_owner.getId());

		if (ownPos.x < _targetPos.x)
			++ownPos.x;
		else if (ownPos.x > _targetPos.x)
			--ownPos.x;

		if (ownPos.y < _targetPos.y)
			++ownPos.y;
		else if (ownPos.y > _targetPos.y)
			--ownPos.y;

		_sim.onMove(_owner.getId(), ownPos);

		return true;
	}

	virtual void updateConsume() override
	{
		Position ownPos = _sim.getPos(_owner.getId());
		_isConsumed = ownPos.x == _targetPos.x && ownPos.y == _targetPos.y;
		if (_isConsumed)
			_sim.onMoveEnd(_owner.getId());
	}
};

//
//class RavenMove : public SimpleMove
//{
//	const Position _targetPos;
//public:
//	SimpleMove(Simulation& sim, const Raven& owner, const Position pos) : DependantAction(sim, owner), _targetPos(pos) {};
//
//	virtual bool execute() const override
//	{
//		// either hardcode move 2, or maybe take it from _owner
//		auto move = 2 || static_cast<const Raven&>(_owner).getMove();
//
//		...
//	}
//
//};