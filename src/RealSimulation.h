#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <format>
#include <optional>

#include <Simulation.h>
#include <Actions/ActionsMaker.h>
#include <Units/UnitsMaker.h>
#include <IO/System/EventLog.hpp>
#include <IO/System/CommandParser.hpp>


class Unit;


class RealSimulation : public Simulation
{
	struct UnitInfo
	{
		Position pos;
		std::unique_ptr<Unit> unit_ptr;
	};

	std::vector<std::vector<uint32_t>> _map;

	std::unordered_map<uint32_t, UnitInfo> _units;

	std::vector<uint32_t> _unitsQueue;

	// this may look scary but its well-defined since we do not accsess the value or anything like that
	ActionsMaker _actionsMaker{ *this };
	UnitsMaker _unitsMaker{ *this, _actionsMaker };

	sw::EventLog _eventLogger;

	uint64_t _tick = 1;

	bool isValid(Position pos) const { return pos.x < _map.size() && pos.y < _map[pos.x].size(); };

public:

	void createMap(uint32_t width, uint32_t height);

	void spawnWarrior(
		uint32_t unitId,
		uint32_t x,
		uint32_t y,
		uint32_t hp,
		uint32_t strength
	);

	void spawnArcher(
		uint32_t unitId,
		uint32_t x,
		uint32_t y,
		uint32_t hp,
		uint32_t strength,
		uint32_t agility,
		uint32_t range
	);

	void addMove(
		uint32_t unitId,
		uint32_t x,
		uint32_t y
	);

	void registerParser(sw::io::CommandParser& parser);

	void run();

	std::optional<Position> getFirstUnit(uint32_t radius, Position pos) const override final;

	Position getPos(uint32_t unitId) const override final
	{
		return _units.at(unitId).pos;
	}

	uint32_t getUnitId(Position pos) const override final
	{
		if (!isValid(pos))
			throw std::runtime_error(std::format("out of bounds - x {}, y {}", pos.x, pos.y));

		return _map[pos.x][pos.y];
	}

	void onAttack(uint32_t attackerId, uint32_t targetId, uint32_t damage) override final;

	void onDeath(uint32_t unitId) override final;

	void onMove(uint32_t unitId, Position pos) override final;

	void onMoveEnd(uint32_t unitId) override final;
};