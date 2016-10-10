#pragma once
#include <Windows.h>
#include <time.h>

#include "../common.h"
#include "Minion/Minion.h"

#pragma pack(push , 1)

struct PacketHeader
{
	PacketHeader()
	{
		netId = 0;
	}

	PacketCmd cmd;
	uint32_t netId;
};

struct GameHeader
{
	GameHeader()
	{
		netId = ticks = 0;
	}

	GameCmd cmd;
	uint32_t netId;
	uint32_t ticks;
};

typedef struct _LockCamera
{
	_LockCamera(bool lock)
	{
		header.cmd = PKT_C2S_LockCamera;
		doLock = lock;
		unk = 0;
	}

	PacketHeader header;
	bool doLock;
	uint32_t unk;
} LockCamera;

typedef struct _StatsConfirm
{
	_StatsConfirm(uint32_t StatId)
	{
		header.cmd = PKT_C2S_StatsConfirm;
		statId = StatId;
	}

	PacketHeader header;
	uint32_t statId;
} StatsConfirm;

typedef struct _BuyItemReq
{
	_BuyItemReq(uint32_t netId, int itemId)
	{
		header.cmd = PKT_C2S_BuyItemReq;
		header.netId = netId;
		id = itemId;
	}

	PacketHeader header;
	uint32_t id;
} BuyItemReq;

typedef struct _BuyItemAns
{
	_BuyItemAns() {
		header.cmd = (PacketCmd)PKT_S2C_BuyItemAns;
		unk2 = 0;
		unk3 = 0x40;
	}
	PacketHeader header;
	uint32_t itemId;
	uint8_t slotId;
	uint8_t stack;
	uint8_t unk2;
	uint8_t unk3;
} BuyItemAns;

typedef struct _SynchBlock
{
	_SynchBlock()
	{
		userId = 0xFFFFFFFFFFFFFFFF;
		netId = 0x1E;
		teamId = 0x64;
		unk2 = 0;
		flag = 0; //1 for bot?
		memset(data1, 0, 64);
		memset(data2, 0, 64);
		unk3 = 0x19;
	}

	uint64_t userId;
	uint16_t netId;
	uint32_t skill1;
	uint32_t skill2;
	uint8_t flag;
	uint32_t teamId;
	uint8_t data1[64];
	uint8_t data2[64];
	uint32_t unk2;
	uint32_t unk3;
} SynchBlock;

struct ClientReady
{
	uint32_t cmd;
	uint32_t playerId;
	uint32_t teamId;
};

typedef struct _SynchVersionAns
{
	_SynchVersionAns()
	{
		header.cmd = PKT_S2C_SynchVersion;
		ok = ok2 = 1;
		memcpy(version, "Version 4.5.0.264 [PUBLIC]", 27);
		memcpy(gameMode, "CLASSIC", 8);
		memset(zero, 0, 2232);
		end1 = 0xE2E0;
		end2 = 0xA0;
	}

	PacketHeader header;
	uint8_t ok;
	uint32_t mapId;
	SynchBlock players[12];
	uint8_t version[27];      //Ending zero so size 26+0x00
	uint8_t ok2;              //1
	uint8_t unknown[228];     //Really strange shit
	uint8_t gameMode[8];
	uint8_t zero[2232];
	uint16_t end1;            //0xE2E0
	uint8_t end2;             //0xA0 || 0x08
} SynchVersionAns;

typedef struct _PingLoadInfo
{
	_PingLoadInfo()
	{
		header.cmd = PKT_C2S_Ping_Load_Info;
		unk1 = 0;
		userId = 0; //How to get this for own player?
		loaded = 1337;
		ping = 1337;
		f3 = 1337;
		unk4 = 0;
	}

	PacketHeader header;
	uint32_t unk1;
	uint64_t userId;
	float loaded;
	float ping;
	float f3;
	uint8_t unk4;
}PingLoadInfo;

typedef struct _CharLoaded
{
	_CharLoaded()
	{
		header.cmd = PKT_C2S_CharLoaded;
	}

	PacketHeader header;
} CharLoaded;

typedef struct _StartGameReq
{
	_StartGameReq()
	{
		header.cmd = PKT_C2S_StartGame;
		unk01 = 0;
		unk02 = 0x5010308;
		unk03 = 0;
		unk04 = 0;
	}

	PacketHeader header;
	uint32_t	 unk01;
	uint32_t	 unk02;
	uint32_t	 unk03;
	uint32_t	 unk04;
} StartGameReq;

typedef struct _UnkDominionPacket
{
	_UnkDominionPacket()
	{
		header.cmd = (PacketCmd)0x56;

		zero[0] = 0;
		zero[1] = 0;
		zero[2] = 0;
		zero[3] = 0;
	}

	PacketHeader header;
	uint32_t zero[4];
} UnkDominionPacket;

typedef struct _OwnHeroSpawned
{
	_OwnHeroSpawned()
	{
		header.cmd = PKT_C2S_OwnHeroSpawned;
		ok = 1;
	}

	PacketHeader header;
	uint8_t ok;
} OwnHeroSpawned;

uint8_t *createDynamicPacket(uint8_t *str, uint32_t size);

typedef struct _LoadScreenInfo
{
	_LoadScreenInfo()
	{
		//Zero this complete buffer
		memset(this, 0, sizeof(_LoadScreenInfo));

		cmd = PKT_S2C_LoadScreenInfo;
		blueMax = redMax = 6;
	}

	uint8_t cmd;
	uint32_t blueMax;
	uint32_t redMax;
	uint64_t bluePlayerIds[6]; //Team 1, 6 players max
	uint8_t blueData[144];
	uint64_t redPlayersIds[6]; //Team 2, 6 players max
	uint8_t redData[144];
	uint32_t bluePlayerNo;
	uint32_t redPlayerNo;
} LoadScreenInfo;

//typedef struct _LoadScreenPlayer
//{
//	static _LoadScreenPlayer* create(PacketCmd cmd, uint8_t *str, uint32_t size)
//	{
//		//Builds packet buffer
//		uint32_t totalSize = sizeof(_LoadScreenPlayer)+size + 1;
//		uint8_t* buf = new uint8_t[totalSize];
//		memset(buf, 0, totalSize);
//
//		//Set defaults
//		_LoadScreenPlayer *packet = (_LoadScreenPlayer *)buf;
//		packet->cmd = cmd;
//		packet->length = size;
//		packet->userId = 0;
//		packet->skinId = 0;
//		memcpy(packet->getDescription(), str, packet->length);
//		return packet;
//	}
//
//	static void destroy(_LoadScreenPlayer *packet)
//	{
//		delete[](uint8_t*)packet;
//	}
//
//	uint8_t cmd;
//	uint64_t userId;
//	uint32_t skinId;
//	uint32_t length;
//	uint8_t description;
//	uint8_t *getDescription()
//	{
//		return &description;
//	}
//
//	uint32_t getPacketLength()
//	{
//		return sizeof(_LoadScreenPlayer)+length;
//	}
//} LoadScreenPlayer;

typedef struct _KeyCheck
{
	_KeyCheck()
	{
		cmd = PKT_KeyCheck;
		partialKey[0] = 0x6F;
		partialKey[1] = 0x39;
		partialKey[2] = 0x00;
		playerNo = 0;
		userId = 0xFFFFFFFF;
		//trash = 40500260;
		trash = 0x0280dfbb;
		checkId = 0;
		//trash2 = 117019928;
		trash2 = 0x077b0ab8;
	}

	uint8_t cmd;
	uint8_t partialKey[3];   //Bytes 1 to 3 from the blowfish key for that client
	uint32_t playerNo;
	uint64_t userId;         //uint8_t testVar[8];   //User id
	uint32_t trash;
	uint64_t checkId;        //uint8_t checkVar[8];  //Encrypted testVar
	uint32_t trash2;
} KeyCheck;

typedef struct _AttentionPing
{
	_AttentionPing()
	{
	}
	_AttentionPing(_AttentionPing *ping)
	{
		cmd = ping->cmd;
		unk1 = ping->unk1;
		x = ping->x;
		y = ping->y;
		z = ping->z;
		unk2 = ping->unk2;
		type = ping->type;
	}

	uint8_t cmd;
	uint32_t unk1;
	float x;
	float y;
	float z;
	uint32_t unk2;
	uint8_t type;
} AttentionPing;

typedef struct _AttentionPingAns
{
	_AttentionPingAns(AttentionPing *ping)
	{
		attentionPing = AttentionPing(ping);
		attentionPing.cmd = PKT_S2C_AttentionPing;
	}

	AttentionPing attentionPing;
	uint32_t response;
} AttentionPingAns;

struct CameraLock
{
	PacketHeader header;
	uint8_t isLock;
	uint32_t padding;
};

typedef struct _ViewReq
{
	uint8_t cmd;
	uint32_t unk1;
	float x;
	float zoom;
	float y;
	float y2;		//Unk
	uint32_t width;	//Unk
	uint32_t height;	//Unk
	uint32_t unk2;	//Unk
	uint8_t requestNo;
} ViewReq;

struct MovementVector
{
	uint16_t x;
	uint16_t y;
};

struct MovementReq
{
	PacketHeader header;
	MoveType type;
	float x;
	float y;
	uint32_t targetNetId;
	uint8_t vectorNo;
	uint32_t netId;
	uint8_t moveData;

	MovementReq(uint32_t netId)
	{
		header.cmd = PKT_C2S_MoveReq;
		header.netId = netId;
		type = MOVE;
		vectorNo = 0;
		this->netId = netId;
		targetNetId = 0;
	}
};

struct MovementAns
{
	MovementAns()
	{
		header.cmd = PKT_S2C_MoveAns;
	}

	GameHeader header;
	uint16_t ok;
	uint8_t vectorNo;
	uint32_t netId;
	uint8_t delta;

	MovementVector *getVector(uint32_t index)
	{
		if (index >= vectorNo)
			return NULL;

		return &((MovementVector *)((hasDelta()) ? (&delta) + 1 : &delta))[index]; //A very fancy way of getting the struct from the dynamic buffer
	}

	bool hasDelta()
	{
		return (delta == 0);
	}

	static uint32_t size(uint8_t vectorNo, bool hasDelta = false)
	{
		return sizeof(MovementAns) + ((vectorNo) * sizeof(MovementVector)) + (hasDelta ? 1 : 0);
	}

	uint32_t size()
	{
		return size(vectorNo, hasDelta());
	}

	static MovementAns *create(uint32_t vectorNo, bool hasDelta = false)
	{
		MovementAns *packet = (MovementAns*)new uint8_t[size(vectorNo)];
		memset(packet, 0, size(vectorNo));
		packet->delta = (hasDelta) ? 0 : 1;
		packet->header.cmd = PKT_S2C_MoveAns;
		packet->header.ticks = clock();
		packet->vectorNo = vectorNo;
		return packet;
	}

	static void destroy(MovementAns *packet)
	{
		delete[](uint8_t*)packet;
	}
};

typedef struct _ViewAns
{
	_ViewAns()
	{
		cmd = PKT_S2C_ViewAns;
		unk1 = 0;
	}

	uint8_t cmd;
	uint32_t unk1;
	uint8_t requestNo;
} ViewAns;

typedef struct _QueryStatusAns
{
	_QueryStatusAns()
	{
		header.cmd = PKT_S2C_QueryStatusAns;
		ok = 1;
	}
	PacketHeader header;
	uint8_t ok;
} QueryStatusAns;

typedef struct _QueryStatusReq
{
	_QueryStatusReq()
	{
		header.cmd = PKT_C2S_QueryStatusReq;
		/*unk = 0;*/
	}

	PacketHeader header;
	//uint32_t unk;
} QueryStatusReq;

typedef struct _SynchVersion
{
	_SynchVersion()
	{
		header.cmd = PKT_C2S_SynchVersion;
	}

	PacketHeader header;
	uint32_t unk1;
	uint32_t unk2;
	uint8_t version[50]; //Dunno how big and when usefull data begins
} SynchVersion;

//typedef struct _WorldSendGameNumber
//{
//	_WorldSendGameNumber()
//	{
//		header.cmd = PKT_World_SendGameNumber;
//		memset(data, 0, sizeof(data1));
//		memset(data, 0, sizeof(data));
//		gameId = 0;
//	}
//
//	PacketHeader header;
//	uint64_t gameId; //_0x0000
//	uint8_t server[5]; //0x0008
//	uint8_t data1[27]; //0x000D
//	uint8_t data[0x80];//0x0028
//} WorldSendGameNumber;

typedef struct _WorldGameNumber
{
	PacketHeader header;
	uint32_t gameId;
	uint32_t zero_1;
	uint8_t  summonerName[128]; //String as well
	uint8_t  platformId[4]; //Represented as string => "EUW1"
	uint8_t  _unknown[28];
} WorldGameNumber;

struct CharacterStats
{
	static uint32_t countBits(uint32_t mask, bool addMask = true)
	{
		uint32_t size = 0;

		for (int i = 0; i < 32; i++)
		{
			if (mask & 1)
				size++;
			mask >>= 1;
		}
		if (addMask && size > 0)
			size++;
		return size;
	}

	uint32_t getSize()
	{
		uint32_t blockNo = 0;
		uint32_t size = sizeof(GameHeader) + 2 + 4;
		uint32_t *offset = &mask;

		//How many blocks do we have?
		if (masterMask & MM_One)   blockNo++;
		if (masterMask & MM_Two)   blockNo++;
		if (masterMask & MM_Three) blockNo++;
		if (masterMask & MM_Four)  blockNo++;
		if (masterMask & MM_Five)  blockNo++;

		for (int i = 0, x = 0; i < blockNo; i++)
		{
			uint32_t bits = countBits(offset[x], false);
			size += bits * 4 + 4;
			x += bits + 1;
		}
		return size;
	}

	static CharacterStats *create(uint32_t blockNo, uint32_t mask)
	{
		switch (blockNo)
		{
		case 1: return create(mask, 0, 0, 0, 0);
		case 2: return create(0, mask, 0, 0, 0);
		case 3: return create(0, 0, mask, 0, 0);
		case 4: return create(0, 0, 0, mask, 0);
		case 5: return create(0, 0, 0, 0, mask);
		default: return NULL;
		}
	}

	static CharacterStats *create(uint32_t one, uint32_t two, uint32_t three, uint32_t four, uint32_t five)
	{
		//Calculate the total size needed
		uint32_t size = sizeof(GameHeader) + 2 + 4; //The header + updateNo + masterMask + netId
		size += countBits(one) * 4;
		size += countBits(two) * 4;
		size += countBits(three) * 4;
		size += countBits(four) * 4;
		size += countBits(five) * 4;

		//Set the defaults
		CharacterStats *stats = (CharacterStats*)new uint8_t[size];
		memset(stats, 0, size);
		stats->header.cmd = (GameCmd)PKT_S2C_CharStats;
		stats->header.ticks = clock();
		stats->updateNo = 1;

		//Set the master mask
		if (one > 0)   stats->masterMask |= MM_One;
		if (two > 0)   stats->masterMask |= MM_Two;
		if (three > 0) stats->masterMask |= MM_Three;
		if (four > 0)  stats->masterMask |= MM_Four;
		if (five > 0)  stats->masterMask |= MM_Five;

		//Set all the masks
		uint32_t *offset = &stats->mask;
		uint32_t x = 0;
		offset[x] = one;
		x += countBits(one, false);
		offset[x] = two;
		x += countBits(two, false);
		offset[x] = three;
		x += countBits(three, false);
		offset[x] = four;
		x += countBits(four, false);
		offset[x] = five;

		return stats;
	}

	void destroy()
	{
		delete[]this;
	}

	void setValue(FieldMaskOne field, float value)
	{
		setValue(1, field, value);
	}

	void setValue(uint8_t blockNo, uint32_t field, float value)
	{
		blockNo -= 1; //We count the fields from 1 to 5
		uint32_t *offset = &mask;

		//Get the offset for the block number
		uint32_t x = 0;
		uint32_t blocks = countBits(masterMask, false);
		if (blocks > 1)
			for (int i = 0; i < blockNo; i++)
				x += countBits(offset[x], false);

		//Get the offset for the field
		for (uint32_t i = 0, a = 0, mask = 1; i < 32; i++)
		{
			if (mask & field)
			{
				memcpy(&offset[x + a + 1], &value, 4);
				a++;
			}
			mask <<= 1;
		}
	}

	GameHeader header;
	uint8_t updateNo;
	uint8_t masterMask;
	uint32_t netId;
	uint32_t mask;
};

struct ChatMessage
{
	uint32_t playerNo;
	ChatType type;
	uint32_t lenght;
	uint8_t msg;

	uint8_t *getMessage()
	{
		return &msg;
	}
};

struct ChatMessageClient
{
	PacketHeader header;
	uint32_t playerNo;
	ChatType type;
	uint32_t lenght;
	uint8_t msg;
};

typedef struct _StatePacket
{
	_StatePacket(PacketCmd state)
	{
		header.cmd = state;
	}
	PacketHeader header;
} StatePacket;
typedef struct _StatePacket2
{
	_StatePacket2(PacketCmd state)
	{
		header.cmd = state;
		nUnk = 0;
	}
	PacketHeader header;
	uint16_t nUnk;
} StatePacket2;

struct FogUpdate2
{
	FogUpdate2()
	{
		header.cmd = PKT_S2C_FogUpdate2;
		header.netId = 0x40000019;
	}
	PacketHeader header;
	float x;
	float y;
	uint32_t radius;
	uint8_t unk1;
};

struct HeroSpawn
{
	PacketHeader header;
	uint32_t netId; //Also found something about locking flag//Perhaps first 4 bits is type and rest is netId?? or something?? //Linked for mastery's (first uitn32, and also animation (looks like it) and possible more) often looks like XX 00 00 40 where XX is around 10-30
	uint32_t playerId; //1-number of players
	uint8_t netNodeID;
	uint8_t botSkillLevel;
	uint8_t teamNumber;
	bool isBot;
	uint8_t spawnPosIndex;
	uint32_t skinNo;
	uint8_t name[128];
	uint8_t type[40];
	float deathDurationRemaining;
	float timeSinceDeath;
	uint32_t unk;
	uint8_t bitField;
};
struct HeroSpawn2 {
	HeroSpawn2()
	{
		header.cmd = (PacketCmd)0xB9;
		ZeroMemory(unk, 30);
		unk[15] = 0x80;
		unk[16] = 0x3F;
		unk1 = 3;
		unk2 = 1;
		f1 = 0x420F9C78;
		f2 = 0x4388C6A5;
		f3 = 0x3F441B7D;
		f4 = 0x3F248DBB;
	}

	PacketHeader header;
	uint8_t unk[30];
	uint8_t unk1;
	uint32_t unk2;
	unsigned long f1;
	unsigned long f2;
	unsigned long f3;
	unsigned long f4;
};
struct HeroSpawn3 {
	HeroSpawn3() {
		header.cmd = (PacketCmd)0xAD;
		unk = 0;
		x = 561;
		y = 561;
	}

	PacketHeader header;
	uint16_t unk;
	float x;
	float y;
};

struct TurretSpawn
{
	TurretSpawn()
	{
		header.cmd = PKT_S2C_TurretSpawn;
		tID = 0;
		unk = 0;
		memset(&name, 0, 29 + 42); //Set name + type to zero
	}

	PacketHeader header;
	uint8_t tID;
	uint16_t unk;
	uint8_t name[29];
	uint8_t type[42];
};

struct DominionTurretSpawn
{
	PacketHeader header;
	uint64_t zero;
	uint32_t unk;
};

class LevelPropSpawn
{
public:
	LevelPropSpawn()
	{
		header.cmd = PKT_S2C_LevelPropSpawn;
		netId = 0;
		memset(&unk, 0, 50 + 64 + 64); //Set name + type to zero
	}
	void SetProp(char* szName, char* szType) {
		header.cmd = PKT_S2C_LevelPropSpawn;
		netId = 0;
		x = 0;
		y = 0;
		memset(&unk, 0, 50);
		memset(&name, 0, 64);
		memset(&type, 0, 64);
		if (szName)
			strcpy((char*)name, szName);
		if (szType)
			strcpy((char*)type, szType);
	}
	PacketHeader header;
	uint32_t netId;
	float x;
	float y;
	uint8_t unk[50];
	uint8_t name[64];
	uint8_t type[64];
};

struct Announce
{
	PacketHeader header;
	uint8_t msg;
	uint64_t unknown;
	uint32_t mapNo;
};

typedef struct _SkillUpPacket
{
	_SkillUpPacket(uint32_t netId, uint8_t skillId)
	{
		header.cmd = PKT_C2S_SkillUp;
		header.netId = netId;
		skill = skillId;
	}

	PacketHeader header;
	uint8_t skill;
} SkillUpPacket;

typedef struct _SkillUpResponse
{
	_SkillUpResponse()
	{
		header.cmd = PKT_S2C_SkillUp;
		level = 0x0000;
	}
	PacketHeader header;
	uint8_t skill;
	uint16_t level; //?
} SkillUpResponse;

typedef struct _EmotionPacket
{
	PacketHeader header;
	uint8_t id;
} EmotionPacket;

typedef struct _EmotionResponse
{
	_EmotionResponse()
	{
		header.cmd = PKT_S2C_Emotion;
	}
	PacketHeader header;
	uint8_t id;
} EmotionResponse;

typedef struct _SetScoreForTeamDominion
{
	PacketHeader header;
	uint32_t teamId; // 100 and 200 available!
	uint32_t score;
} SetScoreForTeamDominion;

typedef struct _CaptureDominionTurret
{
	_CaptureDominionTurret(uint32_t heroNetId)
	{
		header.cmd = PKT_C2S_CaptureDominionTurret;
		header.netId = heroNetId;

		turretId = 0;
	}

	PacketHeader header;
	uint32_t turretId;
} CaptureDominionTurret;

typedef struct _DominionTurretCaptured
{
	PacketHeader header;
	uint32_t turretId;
	uint32_t teamId;
} DominionTurretCaptured;

typedef struct _LevelUp
{
	PacketHeader header;
	uint8_t level;
	uint8_t skillPoints;
} LevelUp;

typedef struct _ChampionDie
{
	PacketHeader header;
	uint32_t goldFromKill;
	uint8_t unk01;
	uint32_t killerId;
	uint8_t unk02;
	uint8_t unk3;
	float respawnTimer;
} ChampionDie;

typedef struct _ChampionRespawn
{
	PacketHeader header;
	float x;
	float y;
	float z;
} ChampionRespawn;

typedef struct _MinionSpawn
{
	PacketHeader header;
	uint32_t unk01;
	uint8_t type; //Should be 0x03
	uint32_t netId_02;
	uint32_t netId_03;
	MinionSpawnPosition spawnPosition;
	uint8_t unk02;
	uint8_t waveNumber;
	MinionSpawnType spawnType;
	bool isRanged;
	uint8_t unk03;
} MinionSpawn;

typedef struct _NpcDie
{
	PacketHeader header;
	uint8_t unk01;
	uint32_t unk02;
	uint16_t unk03;
	uint32_t killerId;
} NpcDie;

struct CastSpell
{
	CastSpell(uint32_t netId, uint8_t spellId, float x, float y, float x2, float y2, uint32_t targetNetId)
	{
		this->header.cmd = PKT_C2S_CastSpell;
		this->header.netId = netId;
		this->spellSlotType = 0x63;
		this->spellSlot = spellId;
		this->x = x;
		this->y = y;
		this->x2 = x2;
		this->y2 = y2;
		this->targetNetId = targetNetId;
	}

	PacketHeader header;
	uint8_t spellSlotType; // 4.18 [deprecated? -> 2 first(highest) bits: 10 - ability or item, 01 - summoner spell]
	uint8_t spellSlot; // 0-3 [0-1 if spellSlotType has summoner spell bits set]
	float x, y;
	float x2, y2;
	uint32_t targetNetId; // If 0, use coordinates, else use target net id
};

struct SetTarget
{
	PacketHeader header;
	uint32_t attackedId;
};

struct BeginAutoAttack
{
	PacketHeader header;
	uint32_t attackedId;

	//There are far more fields
};

struct NextAutoAttack
{
	PacketHeader header;
	uint32_t attackedId;

	//There are far more fields
};

struct StopAutoAttack
{
	PacketHeader header;
};

#pragma pack(pop)