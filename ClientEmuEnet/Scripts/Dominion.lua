-- Lua file gets executed by ClientEmu.exe

-- MAX 10 Players
ownHero = nil;
ownSpawnX = 0;
ownSpawnY = 0;

dominionTurretTable = {};

startSpawnTime = nil;
moveTimer = nil;
captureTimer = nil;

function onLoad()

	--Init random seed
	math.randomseed(os.time())

	print "OmdiBot - Dominion Script loaded!";
end

function onHeroSpawn(netId, summonerName, heroName, spawnX, spawnY)

	print("Registering Hero: " .. summonerName .. " playing " .. heroName);

	if Client:getOwnSummonerName() == summonerName then

		print("Own Hero is spawning!: " .. Client:getOwnSummonerName());

		ownHero = Hero(netId, summonerName, heroName);
		ownSpawnX = spawnX;
		onwSpawnY = spawnY;

	end

end

function onStartGame()
	print "onStartGame called!";

		if ownHero ~= nil then

			if startSpawnTime == nil then

				--Set spawn time
				startSpawnTime = os.time();

				-- Just to make sure we get onCharStats sent
				ownHero:move(0, 0, 0.0);
			end

			-- Turrets not registered? DUDE THE GAME HAS BEEN STARTED WTF!
			if #dominionTurretTable == 0 then
				print("The Game has been started, though we are parsing turrets hm...");
				DominionMgr:parseTurretsFromFile();
			end

		else
			print("Dude, something's wrong with ya HeroSpawn!");

		end


end

--Gets called a few times every seconds (depends on the server, but this should be enough for us :-) )
function onCharStats()

	if startSpawnTime == nil then
		startSpawnTime = os.time();
	end

	if ownHero ~= nil then
		skipWaiting = false;


		if moveTimer == nil then
			moveTimer = os.time();
		end

		if captureTimer == nil then
			captureTimer = os.time();
		end

		-- Register turrets in our lua file
		if #dominionTurretTable == 0 then

			dominionTurrets = DominionMgr:getTurrets();

			for turret in dominionTurrets do
				table.insert(dominionTurretTable, turret);
			end

		end

		-- It could be the case that we waited 2:30 Minutes but got disconnected
		-- If thats the case, we could already have captured a turret
		-- ATTENTION: This could not be the case, fix this later
		if #dominionTurretTable > 0 then

			-- Capture turrets if they aren't captured
			for i = 1, #dominionTurretTable do

				if dominionTurretTable[i].captured == true then
					skipWaiting = true;
					break;
				end

			end

		end


		-- First we stand 2:30 minutes in base and move to spawn (to avoid afk kick).
		-- We do this, because we have to reach a minimum game time of 7 Minutes

		if os.time() - startSpawnTime <= 170 and skipWaiting == false then

			if os.time() - moveTimer < 5 then
				return;
			end

			moveTimer = os.time();

			print("AFK in base for now! - " .. tostring(os.time() - startSpawnTime));
			local x = math.random(0.0, 8000.0);
			local y = math.random(0.0, 8000.0);

			ownHero:move(x, y, 0.0);
			return;


		end

		-- HOST: Captures turrets
		-- BOT : Stays afk in base (move to spawnPos)
		if Client.botType == "HOST" then

			-- Turrets registered? - Logic here
			if #dominionTurretTable > 0 then

				-- Capture turrets if they aren't captured
				for i = 1, #dominionTurretTable do

					if dominionTurretTable[i].captured == false then

						if os.time() - captureTimer < 5 then
							return;
						end

						captureTimer = os.time();

						print("Capture Turret #" .. tostring(i) .. " - 0x" .. string.format("%x", dominionTurretTable[i].netId));
						DominionMgr:captureTurret(ownHero:getNetId(), dominionTurretTable[i].netId);
						return;



					end

				end

					if os.time() - moveTimer < 5 then
						return;
					end

					moveTimer = os.time();

					-- So all turrets captured? - back to base and move to one position - ANTI AFK
					local x = math.random(0.0, 8000.0);
					local y = math.random(0.0, 8000.0);

					print("Job done moving to X: " .. string.format("%f", x) .. " Y: " .. string.format("%f", y));

					ownHero:move(x, y, 0.0);


			else

				print("No Turrets registered! - " .. tostring(os.time()));
				DominionMgr:parseTurretsFromFile();
				--Client:sendExitChat();

			end

		else -- Client.botType == "BOT" - TODO CHECK!

			if os.time() - moveTimer < 5 then
				return;
			end

			moveTimer = os.time();

			print("We are just stupid BOTS - we have to stay in our base :-(! - " .. tostring(os.time()));

			local x = math.random(0.0, 8000.0);
			local y = math.random(0.0, 8000.0);

			ownHero:move(x, y, 0.0);

			--Host bugged?
			if os.time() - startSpawnTime >= 600 then
				Client:sendExitChat();
			end

		end
	else

			print("Cannot continue without ownHeroPtr!");

	end

end

function onGameEnd()
	print("The Game took " .. tostring(os.time() - startSpawnTime) .. "s!");
end
