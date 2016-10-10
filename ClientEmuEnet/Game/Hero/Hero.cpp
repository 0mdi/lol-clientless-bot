#include "Hero.h"

luabind::scope Hero::registerLuaFunctions()
{
	return
		luabind::class_<Hero, Entity>("Hero")
		.def(luabind::constructor<uint32_t, const char*, const char*, uint32_t, uint8_t, uint32_t, bool>())

		//General info
		.def("getHeroName", &Hero::getHeroName)
		.def("getSummonerName", &Hero::getSummonerName)
		.def("getPlayerId", &Hero::getPlayerId)
		.def("getTeamId", &Hero::getTeamId)
		.def("getSkinNo", &Hero::getSkinNo)
		.def("isBot", &Hero::isBot)

		//Player stats
		.def("getLevel", &Hero::getLevel)
		.def("IsAlive", &Hero::isAlive)

		//Actions
		.def("move", &Hero::move)
		.def("attack", &Hero::attack)
		.def("attackMove", &Hero::attackMove)
		.def("buyItem", &Hero::buyItem)
		.def("skillUp", &Hero::skillUp)
		.def("castSpell", &Hero::castSpell)
		;
}

void Hero::addItem(int itemId)
{
	//Does item already exist?
	Item *item = nullptr;

	for (auto itemItr : m_items)
	{
		if (itemItr->getItemId() == itemId)
		{
			item = itemItr;
			break;
		}
	}

	if (item != nullptr)
		item->addStack();
	else
		m_items.push_back(new Item(itemId));
}

void Hero::move(float x, float y, float z)
{
	MovementReq movementRequest(getNetId());
	movementRequest.x = x;
	movementRequest.y = y;

	GameClient::instance().sendEncryptedPacket((uint8_t*)&movementRequest, sizeof(MovementReq), CHL_C2S);
}

void Hero::attackMove(float x, float y, float z)
{
	MovementReq movementRequest(getNetId());
	movementRequest.x = x;
	movementRequest.y = y;
	movementRequest.type = (MoveType)7; //Attack

	GameClient::instance().sendEncryptedPacket((uint8_t*)&movementRequest, sizeof(MovementReq), CHL_C2S);
}

void Hero::attack(Hero *target)
{
	MovementReq movementRequest(getNetId());
	movementRequest.x = 0;
	movementRequest.y = 0;
	movementRequest.type = (MoveType)7; //Attack
	movementRequest.targetNetId = target->getNetId();

	GameClient::instance().sendEncryptedPacket((uint8_t*)&movementRequest, sizeof(MovementReq), CHL_C2S);
}

void Hero::buyItem(int itemId)
{
	BuyItemReq itemReq(getNetId(), itemId);

	GameClient::instance().sendEncryptedPacket((uint8_t*)&itemReq, sizeof(BuyItemReq), CHL_C2S);
}

void Hero::skillUp(uint8_t skillId)
{
	SkillUpPacket skillReq(getNetId(), skillId);
	GameClient::instance().sendEncryptedPacket((uint8_t*)&skillReq, sizeof(SkillUpPacket), CHL_C2S);
}

void Hero::castSpell(uint8_t spellSlot, float x, float y, float x1, float x2, uint32_t targetId)
{
	CastSpell spellCast(getNetId(), spellSlot, x, y, x1, x2, targetId);
	GameClient::instance().sendEncryptedPacket((uint8_t*)&spellCast, sizeof(CastSpell), CHL_C2S);
}