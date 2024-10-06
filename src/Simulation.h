#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <unordered_map>
#include <optional>


struct Position
{
	uint32_t x;
	uint32_t y;
};


class Simulation
{
public:
	Simulation() {};

	virtual ~Simulation() {};

	virtual std::optional<Position> getFirstUnit(uint32_t radius, Position pos) const = 0;

	virtual Position getPos(uint32_t unitId) const = 0;

	virtual uint32_t getUnitId(Position pos) const = 0;

	virtual void onAttack(uint32_t attackerId, uint32_t targetId, uint32_t damage) = 0;

	virtual void onDeath(uint32_t unitId) = 0;

	virtual void onMove(uint32_t unitId, Position pos) = 0;

	virtual void onMoveEnd(uint32_t unitId) = 0;
};
