#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "Game\GameClient.h"

int main(int argc, char *argv[])
{
	SetConsoleTitleA((std::string(argv[6]) + "_c").c_str());

	std::cout << "###########################################" << std::endl;
	std::cout << "League of Legends Game Emulator by Omdihar" << std::endl;

	if (argc != 8)
	{
		std::cout << "Invalid argc value! : " << argc << std::endl;
		return -1;
	}

	std::string serverIp = argv[1];
	std::string serverPort = argv[2];
	std::string encryptionKey = argv[3];
	std::string summonerId = argv[4];
	std::string botType = argv[5];
	//argv[6] = HOST
	std::string levelLimit = argv[7];

	std::cout << "ServerIP: " << serverIp << std::endl;
	std::cout << "ServerPort: " << serverPort << std::endl;
	std::cout << "Encryption Key: " << encryptionKey << std::endl;
	std::cout << "SummonerID: " << summonerId << std::endl;
	std::cout << "BOTTYPE: " << botType << std::endl;
	std::cout << "LEVELLIMIT: " << levelLimit << std::endl;
	std::cout << "###########################################" << std::endl;

	std::stringstream converter;

	converter << serverPort;
	uint16_t serverPortConverted;
	converter >> serverPortConverted;

	converter.clear();

	converter << summonerId;
	uint64_t summonerIdConverted;
	converter >> summonerIdConverted;

	//Blocking call
	GameClient::instance().initialize(serverIp, serverPortConverted, "", encryptionKey, summonerIdConverted, botType, levelLimit);

	return 1;
}