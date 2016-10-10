#include "MinionManager.h"

#include "../Hero/Hero.h"

std::vector<Minion*> MinionManager::getTeamMinions(Hero *hero)
{
	std::vector<Minion*> teamMinions;

	for (auto minion : m_minions)
	{
		//Blue
		if (hero->getTeamId() == 1)
		{
			switch (minion->getSpawnPosition())
			{
			case SPAWN_BLUE_TOP:
			case SPAWN_BLUE_MID:
			case SPAWN_BLUE_BOT:
				teamMinions.push_back(minion);
				break;
			}
		}
		else //Purple
		{
			switch (minion->getSpawnPosition())
			{
			case SPAWN_RED_TOP:
			case SPAWN_RED_MID:
			case SPAWN_RED_BOT:
				teamMinions.push_back(minion);
				break;
			}
		}
	}

	return teamMinions;
}

std::vector<Minion*> MinionManager::getEnemyMinions(Hero *hero)
{
	std::vector<Minion*> enemyMinions;

	for (auto minion : m_minions)
	{
		//Blue
		if (hero->getTeamId() == 1)
		{
			switch (minion->getSpawnPosition())
			{
			case SPAWN_RED_TOP:
			case SPAWN_RED_MID:
			case SPAWN_RED_BOT:
				enemyMinions.push_back(minion);
				break;
			}
		}
		else //Purple
		{
			switch (minion->getSpawnPosition())
			{
			case SPAWN_BLUE_TOP:
			case SPAWN_BLUE_MID:
			case SPAWN_BLUE_BOT:
				enemyMinions.push_back(minion);
				break;
			}
		}
	}

	return enemyMinions;
}

Minion* MinionManager::getMinion(uint32_t minionId)
{
	for (auto minion : m_minions)
	{
		if (minion->getNetId() == minionId)
			return minion;
	}

	return nullptr;
}

void MinionManager::removeMinion(Minion *minion)
{
	auto minionItr = std::find(m_minions.begin(), m_minions.end(), minion);

	//Minion found!
	if (minionItr != m_minions.end())
		m_minions.erase(minionItr);
	else
		std::cout << "MINION NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
}