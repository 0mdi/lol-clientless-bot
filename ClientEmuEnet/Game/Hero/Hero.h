#pragma once
#include "../GameClient.h"
#include "../Entity/Entity.h"
#include "../Item/Item.h"
#include "../Packets.h"

#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>

class Hero : public Entity
{
public:
	Hero(uint32_t netId, std::string summonerName, std::string heroName, uint32_t playerId, uint8_t teamId, uint32_t skinNo, bool is_bot)
		: Entity(netId, summonerName, heroName)
		, m_playerId(playerId)
		, m_teamId(teamId)
		, m_skinNo(skinNo)
		, m_isBot(is_bot)
		, m_isAlive(true)
	{}

	virtual ~Hero()
	{
		for (auto item : m_items)
			delete item;
	}

	static luabind::scope registerLuaFunctions();

	//General info
	std::string getHeroName() { return getType(); }
	std::string getSummonerName() { return getName(); }
	uint32_t getPlayerId() { return m_playerId; }
	uint8_t getTeamId() { return m_teamId; }
	uint32_t getSkinNo() { return m_skinNo; }
	bool isBot() { return m_isBot; }

	//Player stats
	uint8_t getLevel() { return m_level; }
	void setLevel(uint8_t level) { m_level = level; }

	bool isAlive() { return m_isAlive; }
	void setAlive(bool alive) { m_isAlive = alive; }

	std::vector<Item*> getItems() { return m_items; }
	void addItem(int itemId);

	//Implemented
	void move(float x, float y, float z);
	void attackMove(float x, float y, float z);
	void attack(Hero *target);
	void buyItem(int itemId);
	void skillUp(uint8_t skillId);
	void castSpell(uint8_t spellSlot, float x, float y, float x1, float x2, uint32_t targetId);

private:
	uint32_t m_playerId;
	uint8_t m_teamId; // 1 = Blue | 0 = Purple
	bool m_isBot;
	uint32_t m_skinNo;

	//Player stats
	uint8_t m_level;
	bool m_isAlive;

	std::vector<Item*> m_items;
};

Hero* getHeroFromId(uint32_t netId);
Hero* getHeroFromSummonerName(std::string summonerName);