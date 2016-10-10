#pragma once
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>

#include <string>
#include <vector>

class Hero;

class HeroManager
{
public:
	HeroManager()
	{}

	~HeroManager()
	{
		for (auto hero : m_heroes)
			delete hero;
	}

	static luabind::scope registerLuaFunctions()
	{
		return
			luabind::class_ <HeroManager>("HeroManager")
			.def("getHero", (Hero* (HeroManager::*)(const std::string&))&HeroManager::getHero)
			.def("getHero", (Hero* (HeroManager::*)(uint32_t))&HeroManager::getHero);
	}

	Hero* getHero(const std::string &summonerName);
	Hero* getHero(uint32_t netId);

	void addHero(Hero *hero)
	{
		m_heroes.push_back(hero);
	}

private:
	std::vector<Hero*> m_heroes;
};