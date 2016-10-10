#include "HeroManager.h"
#include "Hero.h"

Hero* HeroManager::getHero(const std::string &summonerName)
{
	for (auto hero : m_heroes)
	{
		if (hero->getSummonerName() == summonerName)
			return hero;
	}

	return nullptr;
}

Hero* HeroManager::getHero(uint32_t netId)
{
	for (auto hero : m_heroes)
	{
		if (hero->getNetId() == netId)
			return hero;
	}

	return nullptr;
}