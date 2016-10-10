#pragma once
#include "../GameClient.h"
#include "../Packets.h"

#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdio>

#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/iterator_policy.hpp>

class Dominion
{
public:
	struct Turret
	{
		Turret()
		{
			netId = 0;
			captured = false;
			teamId = -1;
		}

		uint32_t netId;
		bool captured;
		uint32_t teamId;
	};

	Dominion()
		: m_ready(false)
	{}

	virtual ~Dominion();

	static luabind::scope registerLuaFunctions();

	void registerTurret(uint32_t netId);
	void updateScore(uint32_t teamId, uint32_t score);
	void onTurretCaptured(uint32_t netId, uint32_t teamId);

	std::vector<Turret*>& getTurrets();
	uint32_t getScore(uint32_t teamId);

	void captureTurret(uint32_t heroNetId, uint32_t turretNetId);

	void parseTurretsFromFile();

private:
	std::vector<Turret*> m_dominionTurrets;

	uint32_t m_teamOneScore;
	uint32_t m_teamTwoScore;

public:
	bool m_ready;
};