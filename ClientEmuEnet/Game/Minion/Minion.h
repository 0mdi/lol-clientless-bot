#pragma once
#include "../Entity/Entity.h"

enum MinionSpawnPosition : uint32_t {
	SPAWN_BLUE_TOP = 0xeb364c40,
	SPAWN_BLUE_BOT = 0x53b83640,
	SPAWN_BLUE_MID = 0xb7717140,
	SPAWN_RED_TOP = 0xe647d540,
	SPAWN_RED_BOT = 0x5ec9af40,
	SPAWN_RED_MID = 0xba00e840
};

enum MinionSpawnType : uint8_t {
	MINION_TYPE_MELEE = 0x00,
	MINION_TYPE_CASTER = 0x03,
	MINION_TYPE_CANNON = 0x02,
	MINION_TYPE_SUPER = 0x01
};

class Minion : public Entity
{
public:
	Minion(uint32_t netId, MinionSpawnPosition spawnPos, MinionSpawnType spawnType)
		: Entity(netId, "MINION", "MINION")
	{}

	static luabind::scope registerLuaFunctions()
	{
		return
			luabind::class_<Minion, Entity>("Minion")
			.def("getSpawnPosition", &Minion::getSpawnPosition)
			.def("getSpawnType", &Minion::getSpawnType)
			;
	}

	MinionSpawnPosition getSpawnPosition() { return m_spawnPosition; }
	MinionSpawnType getSpawnType() { return m_spawnType; }
	uint8_t getTeamId()
	{
		switch (m_spawnPosition)
		{
		case SPAWN_BLUE_TOP:
		case SPAWN_BLUE_MID:
		case SPAWN_BLUE_BOT:
			return 1;
			break;

		case SPAWN_RED_TOP:
		case SPAWN_RED_MID:
		case SPAWN_RED_BOT:
			return 0;
			break;
		}

		return 0xFF;
	}

private:
	MinionSpawnPosition m_spawnPosition;
	MinionSpawnType m_spawnType;
};