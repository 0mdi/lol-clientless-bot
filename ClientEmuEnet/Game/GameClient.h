#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdint.h>
#include <cstdio>
#include <fstream>
#include <limits>

#include "../ENET/enet.h"

#include "../Core/blowfish/blowfish.h"
#include "../Core/base64.h"
#include "../Core/Console.h"
#include "../Core/MemoryStream.h"

#include "Packets.h"

#include "Hero/Hero.h"
#include "Hero/HeroManager.h"
#include "Minion/Minion.h"
#include "Minion/MinionManager.h"

#include "Dominion/Dominion.h"

extern "C"
{
#include "../lua.h"
#include "../lualib.h"
#include "../lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>

#include "../detours.h"

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "lua5.1.lib")
#pragma comment(lib, "luabind.lib")
#pragma comment(lib, "detours.lib")

namespace con = JadedHoboConsole;

class Hero;
class Dominion;

class GameClient
{
public:
	static GameClient& instance();

	void	initialize(const std::string &serverIp, uint16_t serverPort, const std::string &handshakeToken, const std::string &encryptionKey, double summonerId, std::string botType, std::string levelLimit);

	void	sendPacket(uint8_t *data, uint32_t length, uint8_t channel, uint32_t flag = 1);
	void	sendEncryptedPacket(uint8_t *data, uint32_t length, uint8_t channel, uint32_t flag = 1);

	/*luabind::object getHeroes(lua_State *luaState);*/
	std::string getOwnSummonerName();
	void lockCamera(bool doLock);

	void sendExitChat();
	void log(const char *msg);

private:
	GameClient();
	~GameClient();

	void registerLuaFunctions();

	static DWORD __stdcall queryThread(void*);
	static DWORD __stdcall loadingScreenThread(void*);

	void handlePackets(ENetPacket *packet, bool doDecrypt = true);

	void handleBatch(ENetPacket *packet);
	void handleKeyCheck(ENetPacket *packet);
	void handleSendGameNumber(ENetPacket *packet);
	void handleQueryStatusAns(ENetPacket *packet);
	void handleHeroSpawn(ENetPacket *packet);
	void handleSynchVersion(ENetPacket *packet);
	void handlePingLoadInfo(ENetPacket *packet);
	void handleStartGame(ENetPacket *packet);
	void handleStartSpawn(ENetPacket *packet);
	void handleEndSpawn(ENetPacket *packet);
	void handleLevelPropSpawn(ENetPacket *packet);
	void handleTurretSpawn(ENetPacket *packet);
	void handleMoveAns(ENetPacket *packet);
	void handleSetScoreForTeamDominion(ENetPacket *packet);
	void handleAnnounce(ENetPacket *packet);
	void handleCharStats(ENetPacket *packet);
	void handleSpawnDominionTurret(ENetPacket *packet);
	void handleDominionTurretCaptured(ENetPacket *packet);
	void handleEndGame(ENetPacket *packet);
	void handlePlayerDisconnected(ENetPacket *packet);
	void handleLevelUp(ENetPacket *packet);
	void handleChampionDie(ENetPacket *packet);
	void handleChampionRespawn(ENetPacket *packet);
	void handleBuyItem(ENetPacket *packet);
	void handleObjectSpawn(ENetPacket *packet);
	void handleNpcDie(ENetPacket *packet);
	void handleBeginAutoAttack(ENetPacket *packet);
	void handleNextAutoAttack(ENetPacket *packet);
	void handleStopAutoAttack(ENetPacket *packet);

	void handleChatBoxMessage(ENetPacket *packet);

private:
	ENetHost *m_pClient;

	BlowFish *m_pBlowfish;
	Dominion *m_pDominionMgr;

	std::ofstream m_logFile;

	std::string	m_ownSummonerName;

	std::vector<uint32_t> m_dominionTurrets;

	double m_summonerId;

	bool m_gameStarted;
	bool m_registeredFunctions;
	bool m_versionSynched;
	bool m_charLoaded;
	bool m_ownHeroSpawned;

public:
	//God pls no
	ENetPeer *m_pServerPeer;

	std::string m_botType;
	std::string m_levelLimit;
	uint32_t m_gameId;

	bool m_stopQuery;
	bool m_queryThreadStarted;
	bool m_loadingScreenThreadStarted;

	lua_State *m_luaState;
	HeroManager m_heroManager;
	MinionManager m_minionManager;
};