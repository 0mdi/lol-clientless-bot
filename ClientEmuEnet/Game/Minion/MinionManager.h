#pragma once
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>

#include <string>
#include <vector>

#include "Minion.h"

class Hero;

class MinionManager
{
public:
	MinionManager()
	{}

	~MinionManager()
	{
	}

	static luabind::scope registerLuaFunctions()
	{
		return
			luabind::class_ <MinionManager>("MinionManager")
			.def("getTeamMinions", &MinionManager::getTeamMinions)
			.def("getEnemyMinions", &MinionManager::getEnemyMinions)
			.def("getMinions", &MinionManager::getMinions)
			.def("getMinion", &MinionManager::getMinion)
			;
	}

	std::vector<Minion*> getTeamMinions(Hero *hero);
	std::vector<Minion*> getEnemyMinions(Hero *hero);
	std::vector<Minion*>& getMinions() { return m_minions; }
	Minion* getMinion(uint32_t minionId);
	void addMinion(Minion *minion) { m_minions.push_back(minion); }
	void removeMinion(Minion *minion);

private:
	std::vector<Minion*> m_minions;
};