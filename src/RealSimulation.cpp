#include <format>
#include <stdexcept>

#include <RealSimulation.h>
#include <Utils.hpp>
#include <Units/Unit.h>

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/March.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitAttacked.hpp>


using namespace sw;


void RealSimulation::createMap(uint32_t width, uint32_t height)
{
	if (!_map.empty())
		throw std::runtime_error("cant recreate a map");

	_map = { width, std::vector<uint32_t>(height) };

	_eventLogger.log(_tick, io::MapCreated{ width, height });
}


void RealSimulation::spawnWarrior(
	uint32_t unitId,
	uint32_t x,
	uint32_t y,
	uint32_t hp,
	uint32_t strength
)
{
	if (_units.contains(unitId))
		throw std::runtime_error(std::format("unit already exists - unitId {}", unitId));

	if (!isValid({ x, y }))
		throw std::runtime_error(std::format("out of bounds - unitId {}, x {}, y {}", unitId, x, y));

	_map[x][y] = unitId;

	_units.emplace(unitId, UnitInfo{ x, y, _unitsMaker.makeWarror(unitId, hp, strength) });

	_unitsQueue.push_back(unitId);

	_eventLogger.log(_tick, io::UnitSpawned{ unitId, "Warrior", x, y });
}


void RealSimulation::spawnArcher(
	uint32_t unitId,
	uint32_t x,
	uint32_t y,
	uint32_t hp,
	uint32_t strength,
	uint32_t agility,
	uint32_t range
)
{
	if (_units.contains(unitId))
		throw std::runtime_error(std::format("unit already exists - unitId {}", unitId));

	if (!isValid({ x, y }))
		throw std::runtime_error(std::format("out of bounds - unitId {}, x {}, y {}", unitId, x, y));

	_map[x][y] = unitId;

	_units.emplace(unitId, UnitInfo{ x, y, _unitsMaker.makeArcher(unitId, hp, strength, agility, range) });

	_unitsQueue.push_back(unitId);

	_eventLogger.log(_tick, io::UnitSpawned{ unitId, "Archer", x, y });
}


void RealSimulation::addMove(
	uint32_t unitId,
	uint32_t x,
	uint32_t y
)
{
	auto it = _units.find(unitId);
	if (it == _units.end())
		throw std::runtime_error(std::format("addMove: no such unit - unitId {}", unitId));

	auto& unit = *it->second.unit_ptr;

	unit.addAction(_actionsMaker.makeMove(unit, { x, y }));

	const Position& pos = it->second.pos;

	_eventLogger.log(_tick, io::MarchStarted{ unitId, pos.x, pos.y, x, y });
}


void RealSimulation::registerParser(sw::io::CommandParser& parser)
{
	parser.add<io::CreateMap>(
		[this](io::CreateMap command)
		{
			createMap(command.width, command.height);
		}
	);

	parser.add<io::SpawnWarrior>(
		[this](io::SpawnWarrior command)
		{
			spawnWarrior(
				command.unitId,
				command.x,
				command.y,
				command.hp,
				command.strength
			);
		}
	);

	parser.add<io::SpawnArcher>(
		[this](io::SpawnArcher command)
		{
			spawnArcher(
				command.unitId,
				command.x,
				command.y,
				command.hp,
				command.strength,
				command.agility,
				command.range
			);
		}
	);

	parser.add<io::March>(
		[this](io::March command)
		{
			addMove(
				command.unitId,
				command.targetX,
				command.targetY
			);
		}
	);
}


void RealSimulation::run()
{
	bool needToContinue = true;
	while (needToContinue)
	{
		++_tick;
		needToContinue = false;
		for (uint32_t unitId : std::vector(_unitsQueue))
		{
			auto it = _units.find(unitId);
			if (it == _units.end())
				continue; // dead unit
			bool isExecuted = it->second.unit_ptr->executeActions();
			needToContinue |= isExecuted;
			needToContinue &= _units.size() > 1;
		}
	}
}


std::optional<Position> RealSimulation::getFirstUnit(uint32_t radius, Position pos) const
{
	std::function<bool(const uint32_t&)> pred = [](const uint32_t& unitId) { return bool(unitId); };
	auto op = getFirstElement(radius, pos.x, pos.y, _map, pred);
	if (op)
		return std::optional<Position>{ std::in_place, op->first, op->second };
	else
		return {};
}


void RealSimulation::onAttack(uint32_t attackerId, uint32_t targetId, uint32_t damage)
{
	if (!_units.contains(attackerId))
		throw std::runtime_error(std::format("onAttack: no such attacker - unitId {}", attackerId));

	auto it = _units.find(targetId);
	if (it == _units.end())
		throw std::runtime_error(std::format("onAttack: no such target - unitId {}", targetId));

	auto& unit = *it->second.unit_ptr;

	unit.removeHp(damage);
	auto hp = unit.getHp();

	_eventLogger.log(_tick, io::UnitAttacked{ attackerId, targetId, damage, hp });

	if (!hp)
		onDeath(targetId);
}

void RealSimulation::onDeath(uint32_t unitId)
{
	auto it = _units.find(unitId);
	if (it == _units.end())
		throw std::runtime_error(std::format("onDeath: no such unit - unitId {}", unitId));

	const auto& pos = it->second.pos;

	_map[pos.x][pos.y] = 0u;
	_units.erase(it);
	_unitsQueue.erase(std::find(_unitsQueue.begin(), _unitsQueue.end(), unitId));

	_eventLogger.log(_tick, io::UnitDied{ unitId });
}

void RealSimulation::onMove(uint32_t unitId, Position pos)
{
	if (!isValid(pos))
		throw std::runtime_error(std::format("invaid pos - unitId {} x {}, y", unitId, pos.x, pos.y));

	auto it = _units.find(unitId);
	if (it == _units.end())
		throw std::runtime_error(std::format("onMove: no such unit - unitId {}", unitId));

	auto& ownPos = it->second.pos;

	_map[ownPos.x][ownPos.y] = 0u;
	_map[pos.x][pos.y] = unitId;
	ownPos.x = pos.x;
	ownPos.y = pos.y;

	_eventLogger.log(_tick, io::UnitMoved{ unitId, pos.x, pos.y});
}

void RealSimulation::onMoveEnd(uint32_t unitId)
{
	Position pos = getPos(unitId);

	_eventLogger.log(_tick, io::MarchEnded{ unitId, pos.x, pos.y});
}

