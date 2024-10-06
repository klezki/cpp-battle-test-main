#pragma once

#include <iosfwd>
#include <cstdint>

namespace sw::io
{
	struct SpawnArcher
	{
		constexpr static const char* Name = "SPAWN_ARCHER";

		uint32_t unitId {};
		uint32_t x {};
		uint32_t y {};
		uint32_t hp {};
		uint32_t agility {};
		uint32_t strength {};
		uint32_t range {};


		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("x", x);
			visitor.visit("y", y);
			visitor.visit("hp", hp);
            visitor.visit("agility", agility);
            visitor.visit("strength", strength);
            visitor.visit("range", range);
		}
	};
}