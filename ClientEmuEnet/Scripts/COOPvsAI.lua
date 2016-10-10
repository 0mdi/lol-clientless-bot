-- Lua file gets executed by ClientEmu.exe
ownHero = nil;
moveTimer = nil;
moveLimit = 1;

function onLoad()

	--Init random seed
	math.randomseed(os.time())

	print("OmdiBot - COOPvsAI Script loaded!");
end

function onHeroSpawn(netId, summonerName, heroName, spawnX, spawnY)

	print("Registering Hero: " .. summonerName .. " playing " .. heroName);

	if Client:getOwnSummonerName() == summonerName then

		print("Own Hero is spawning!: " .. Client:getOwnSummonerName());
		print("SpawnX: " .. tostring(spawnX) .. " SpawnY: " .. tostring(spawnY));

		ownHero = Hero(netId, summonerName, heroName);

	end

end

function onStartGame()

	print("[COOPvsAI.lua]  onStartGame called!");

	if ownHero ~= nil then

		ownHero:move(0.0, 0.0, 1.0);

	end

end

function onCharStats()

	if ownHero ~= nil then

		if moveTimer == nil then
			moveTimer = os.time();
		end

		if os.time() - moveTimer < moveLimit then
			return;
		end

		moveTimer = os.time();
		moveLimit = math.random(1, 5);

		local x = math.random(0.0, 18000.0);
		local y = math.random(0.0, 18000.0);

		print("AFK MOVING! = {" .. tostring(x) .. ", " .. tostring(y) .. "}");

		ownHero:move(x, y, 0.0);

	end

end

function onGameEnd()
	print("The Game has ended!");
end
