#pragma once
#include <stdint.h>

enum GameCmd : uint8_t
{
	PKT_S2C_MoveAns = 0x67,
};

//Currently attuned to Live 4.5.0.264
enum PacketCmd : uint8_t
{                                               //Channel //Type
	PKT_KeyCheck = 0x00, //0      //UPDATED

	PKT_S2C_NextAutoAttack = 0x0C,

	PKT_S2C_EndSpawn = 0x12, //? //UPDATED
	PKT_C2S_QueryStatusReq = 0x14, //1 //UPDATED
	PKT_S2C_SkillUp = 0x16, //3 //UPDATED
	PKT_C2S_Ping_Load_Info = 0x16, //1  //UPDATED
	PKT_S2C_BeginAutoAttack = 0x1A,
	/*PKT_S2C_DominionTurretCaptured = 0x1C,*/

	PKT_S2C_ViewAns = 0x2C, //3  //UPDATED
	PKT_C2S_ViewReq = 0x2E, //1 //UPDATED
	PKT_S2C_ChampionRespawn = 0x2F,

	PKT_S2C_StopAutoAttack = 0x34,
	PKT_C2S_SkillUp = 0x39, //1 //UPDATED
	PKT_C2S_CaptureDominionTurret = 0x3A,
	PKT_S2C_LevelUp = 0x3F,
	PKT_S2C_AttentionPing = 0x40, //3 //UPDATED

	PKT_S2C_Emotion = 0x42, //3 UPDATED
	PKT_C2S_OwnHeroSpawned = 0x47, //1
	PKT_C2S_Emotion = 0x48, //1 UPDATED
	PKT_S2C_HeroSpawn = 0x4C, //3 UPDATED
	PKT_S2C_Announce = 0x4D, //3
	PKT_S2C_SpawnDominionTurret = 0x50,

	PKT_C2S_StartGame = 0x52, //1 //UPDATED
	//PKT_S2C_SynchVersion = 0x53, //3 //UPDATED
	PKT_S2C_SynchVersion = 0x54, //3 //UPDATED
	PKT_C2S_ScoreBord = 0x56, //1 //UPDATED
	PKT_C2S_AttentionPing = 0x57, //1 //UPDATED
	PKT_S2C_StartGame = 0x5C, //? //UPDATED
	PKT_S2C_ChampionDie = 0x5E,

	PKT_S2C_StartSpawn = 0x62, //? //UPDATED
	//PKT_C2S_OpenShop = 0x63, //1 - DEPRECATED
	PKT_C2S_ClientReady = 0x65, //6 //UPDATED ?
	PKT_S2C_LoadHero = 0x66, //6 //UPDATED
	PKT_S2C_LoadName = 0x67, //6 //UPDATED
	PKT_S2C_LoadScreenInfo = 0x68, //6 //UPDATED
	PKT_ChatBoxMessage = 0x69, //5 //UPDATED
	PKT_S2C_BuyItemAns = 0x6F, //3 //UPDATED

	PKT_C2S_MoveReq = 0x72, //1 //UPDATED
	PKT_C2S_MoveConfirm = 0x77, //1 //UPDATED

	PKT_C2S_LockCamera = 0x81, //1 //UPDATED
	PKT_C2S_BuyItemReq = 0x82, //1 //UPDATED
	//PKT_S2C_QueryStatusAns = 0x87, //? //UPDATED
	PKT_S2C_QueryStatusAns = 0x88, //? //UPDATED
	PKT_C2S_Exit = 0x8F, //1 //UPDATED

	PKT_World_SendGameNumber = 0x92, //3 //UPDATED
	PKT_S2C_Ping_Load_Info = 0x95, //4 //UPDATED
	PKT_S2C_PlayerDisconnected = 0x98,
	PKT_C2S_CastSpell = 0x9A,
	PKT_S2C_TurretSpawn = 0x9D, //3 UPDATED

	PKT_C2S_Surrender = 0xA4, //1 //UPDATED
	//PKT_C2S_GameNumberReq = 0xA6, //1 //DEPRECATED
	PKT_C2S_StatsConfirm = 0xA8, //1 UPDATED
	PKT_C2S_Click = 0xAF, //1 //UPDATED

	PKT_S2C_ObjectSpawn = 0xBA,
	PKT_C2S_SynchVersion = 0xBD, //1 //UPDATED
	PKT_C2S_CharLoaded = 0xBE, //1 //UPDATED

	PKT_S2C_CharStats = 0xC4, //4 //UPDATED
	PKT_S2C_EndGame = 0xC6,
	PKT_S2C_FogUpdate2 = 0x24, //3 //UPDATED?
	PKT_S2C_LevelPropSpawn = 0xD0, //3 UPDATED

	PKT_S2C_SetScoreForTeamDominion = 0xD4,
	PKT_S2C_DominionTurretCaptured = 0xD7,

	PKT_S2C_NPC_Die = 0xFE,
	PKT_Batch = 0xFF, //3

	//PKT_KeyCheck = 0x00, //0      //UPDATED
	//PKT_ChatBoxMessage = 0x68, //5 //UPDATED
	//PKT_S2C_StartSpawn = 0x61, //? //UPDATED
	//PKT_S2C_EndSpawn = 0x11, //? //UPDATED
	//PKT_C2S_QueryStatusReq = 0x14, //1 //UPDATED
	//PKT_S2C_SkillUp = 0x15, //3 //UPDATED
	//PKT_C2S_Ping_Load_Info = 0x16, //1  //UPDATED
	//PKT_S2C_ViewAns = 0x2B, //3  //UPDATED
	//PKT_C2S_ViewReq = 0x2D, //1 //UPDATED
	//PKT_C2S_SkillUp = 0x38, //1 //UPDATED
	//PKT_S2C_AttentionPing = 0x3F, //? //UPDATED
	//PKT_S2C_Emotion = 0x41, //? UPDATED
	//PKT_C2S_OwnHeroSpawned = 0x46, //1
	//PKT_C2S_Emotion = 0x4E,
	//PKT_S2C_Announce = 0x4C, //3
	//PKT_S2C_HeroSpawn = 0x4B, //UPDATED
	//PKT_S2C_TurretSpawn = 0x9C, //UPDATED
	//PKT_S2C_LevelPropSpawn = 0xCF, //UPDATED
	//PKT_C2S_StartGame = 0x51, //1 //UPDATED
	//PKT_S2C_SynchVersion = 0x53, //3 //UPDATED
	//PKT_C2S_AttentionPing = 0x56,
	//PKT_C2S_ScoreBord = 0x5C, //1
	//PKT_S2C_StartGame = 0x5B, //? //UPDATED
	//PKT_C2S_OpenShop = 0x63, //1
	//PKT_C2S_ClientReady = 0x64, //6 //UPDATED ?
	//PKT_S2C_LoadHero = 0x65, //6 //UPDATED
	//PKT_S2C_LoadName = 0x66, //6 //UPDATED
	//PKT_S2C_LoadScreenInfo = 0x67, //6 //UPDATED
	//PKT_S2C_MoveAnswer = 0x60, //4 //UPDATED
	//PKT_C2S_MoveReq = 0x71, //1 //UPDATED
	//PKT_C2S_MoveConfirm = 0x7D, //1
	//PKT_C2S_ReqBuyItem = 0x82, //1 //UPDATED
	//PKT_C2S_LockCamera = 0x88, //1
	//PKT_S2C_QueryStatusAns = 0x87, //? //UPDATED
	//PKT_C2S_Exit = 0x96, //1
	//PKT_World_SendGameNumber = 0x91, //3
	//PKT_S2C_Ping_Load_Info = 0x94, //4 //UPDATED
	//PKT_C2S_GameNumberReq = 0xA6, //1
	//PKT_C2S_Surrender = 0xAE, //1
	//PKT_C2S_StatsConfirm = 0xB2, //1
	//PKT_C2S_Click = 0xAE, //1 //UPDATED
	//PKT_S2C_FogUpdate2 = 0xC6, //3
	//PKT_C2S_SynchVersion = 0xBC, //1 //UPDATED
	//PKT_C2S_CharLoaded = 0xBD, //1 //UPDATED
	//PKT_S2C_CharStats = 0xD0, //4
	//PKT_Batch = 0xFF, //3
};

enum MoveType : uint8_t
{
	EMOTE = 1,
	MOVE = 2,
	STOP = 10,
};

enum ChatType : uint32_t
{
	CHAT_ALL = 0,
	CHAT_TEAM = 1,
};

#define CHL_MAX = 7
enum Channel : uint8_t
{
	CHL_HANDSHAKE = 0,
	CHL_C2S = 1,
	CHL_GAMEPLAY = 2,
	CHL_S2C = 3,
	CHL_LOW_PRIORITY = 4,
	CHL_COMMUNICATION = 5,
	CHL_LOADING_SCREEN = 7,
};

enum Spell : uint32_t
{
	SPL_Revive = 0x05C8B3A5,
	SPL_Smite = 0x065E8695,
	SPL_Exhaust = 0x08A8BAE4,
	SPL_Barrier = 0x0CCFB982,
	SPL_Teleport = 0x004F1364,
	SPL_Ghost = 0x064ACC95,
	SPL_Heal = 0x0364AF1C,
	SPL_Cleanse = 0x064D2094,
	SPL_Clarity = 0x03657421,
	SPL_Ignite = 0x06364F24,
	SPL_Promote = 0x0410FF72,
	SPL_Clair = 0x09896765,
	SPL_Flash = 0x06496EA8,
	SPL_Test = 0x0103D94C,
};

enum MasterMask : uint8_t
{
	MM_One = 0x0001,
	MM_Two = 0x0002,
	MM_Three = 0x0004,
	MM_Four = 0x0008,
	MM_Five = 0x0010,
};

enum FieldMaskOne : uint32_t
{
	FM1_Gold = 0x00000001,
};

enum FieldMaskTwo : uint32_t
{
	FM2_Armor = 0x00000100,
	FM2_Hp5 = 0x00000400,
	FM2_Mp5 = 0x00000800,
	FM2_Ap = 0x00008000,
};

enum FieldMaskThree : uint32_t
{
};

enum FieldMaskFour : uint32_t
{
	FM4_CurrentHp = 0x00000001,
	FM4_CurrentMana = 0x00000002,
	FM4_MaxHp = 0x00000004,
	FM4_MaxMp = 0x00000008,
	FM4_exp = 0x00000010,
	FM4_Vision1 = 0x00000100,
	FM4_Vision2 = 0x00000200,
	FM4_Speed = 0x00000400,
	FM4_ModelSize = 0x00000800,
};

enum FieldMaskFive : uint32_t
{
};

enum StatsIds : uint32_t
{
	STI_Movement = 0x00000400,
	STI_Gold = 0x00000001,
	//STI_Health = 0x00000001,
	STI_Exp = 0x00000010,
	STI_Health = 0x00000005,
	STI_AbilityPower = 0x00008000,
	STI_AttackDamage = 0x00002000,
	STI_Mana = 0x0000000a,
};