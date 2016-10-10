#include "Dominion.h"

luabind::scope Dominion::registerLuaFunctions()
{
	return
		luabind::class_<Dominion>("Dominion")
		.def_readonly("ready", &Dominion::m_ready)
		.def("getTurrets", &Dominion::getTurrets, luabind::return_stl_iterator)
		.def("getScore", &Dominion::getScore)
		.def("captureTurret", &Dominion::captureTurret)
		.def("parseTurretsFromFile", &Dominion::parseTurretsFromFile),

		luabind::class_<Dominion::Turret>("DominionTurret")
		.def_readwrite("netId", &Turret::netId)
		.def_readwrite("captured", &Turret::captured)
		.def_readwrite("teamId", &Turret::teamId)
		;
}

bool file_exists(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

Dominion::~Dominion()
{
	auto turretFileName = "Logs/Dominion/" + std::to_string(GameClient::instance().m_gameId) + ".turrets";
	std::remove(turretFileName.c_str());
}

void Dominion::registerTurret(uint32_t netId)
{
	static uint8_t turretCounter = 0;
	static bool turretFileWriter = false;

	//We have 5 Dominion Turrets - so return now!
	if (turretCounter == 5)
		return;

	//Does this turret already exist? - this should not happen - FATAL ERROR
	/*if (std::find(m_dominionTurrets.begin(), m_dominionTurrets.end(), netId) != m_dominionTurrets.end())
		throw std::runtime_error("Dominion::registerDominionTurret - Turret already exists!");*/

		//Turret info file already exists for this game?
	auto turretFileName = "Logs/Dominion/" + std::to_string(GameClient::instance().m_gameId) + ".turrets";

	if (!file_exists(turretFileName.c_str()) || turretFileWriter)
	{
		turretFileWriter = true;

		std::ofstream turretFile(turretFileName, std::ofstream::app | std::ofstream::binary);

		if (turretFile.is_open())
		{
			turretFile << netId << std::endl;
			turretFile.close();
		}
	}

	Turret *newTurret = new Turret();
	newTurret->netId = netId;

	m_dominionTurrets.push_back(newTurret);

	++turretCounter;
	if (turretCounter == 5)
		m_ready = true;
}

void Dominion::updateScore(uint32_t teamId, uint32_t score)
{
	//Team one
	if (teamId == 100)
		m_teamOneScore = score;
	else if (teamId == 200)
		m_teamTwoScore = score;
}

std::vector<Dominion::Turret*>& Dominion::getTurrets()
{
	return m_dominionTurrets;
}

uint32_t Dominion::getScore(uint32_t teamId)
{
	if (teamId == 100)
		return m_teamOneScore;
	else if (teamId == 200)
		return m_teamTwoScore;

	return 501; // ERROR
}

void Dominion::captureTurret(uint32_t heroNetId, uint32_t turretNetId)
{
	CaptureDominionTurret captureTurret(heroNetId);
	captureTurret.turretId = turretNetId;

	GameClient::instance().sendEncryptedPacket((uint8_t*)&captureTurret, sizeof(CaptureDominionTurret), CHL_C2S);
}

void Dominion::onTurretCaptured(uint32_t netId, uint32_t teamId)
{
	for (auto turret : m_dominionTurrets)
	{
		if (turret->netId == netId)
		{
			if (teamId == 200)
				turret->teamId = 0;

			if (teamId == 100)
				turret->teamId = 1;

			turret->captured = true;
			return;
		}
	}

	//throw std::runtime_error("Dominion::onTurretCaptured: Unknown TurretID");
}

void Dominion::parseTurretsFromFile()
{
	auto turretFileName = "Logs/Dominion/" + std::to_string(GameClient::instance().m_gameId) + ".turrets";

	if (file_exists(turretFileName.c_str()))
	{
		std::ifstream turretFile(turretFileName);

		if (turretFile.is_open())
		{
			for (std::string turretIdString; std::getline(turretFile, turretIdString); )
			{
				if (!turretIdString.empty())
				{
					std::stringstream ss;
					ss << turretIdString;

					uint32_t turretId;
					ss >> turretId;

					std::cout << "PARSED TurretID: 0x" << std::hex << turretId << std::dec << std::endl;

					registerTurret(turretId);
				}
			}

			turretFile.close();
		}
	}
}