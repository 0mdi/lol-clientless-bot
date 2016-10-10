using BlubLib.Configuration;
using com.riotgames.platform.gameinvite.contract;
using LegendaryClient.Logic;
using LegendaryClient.Logic.Riot;
using LegendaryClient.Logic.Riot.Platform;
using RtmpSharp.IO;
using RtmpSharp.Messaging;
using RtmpSharp.Net;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Net.Http;
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PvPNetClient
{
    internal class Program
    {
        internal static string userName;
        internal static string roomName;
        internal static string roomNameTwo;
        internal static string hostName;
        internal static string hostNameTwo;

        public static string[] argsCopy;

        internal static bool handlersRegistered = false;
        internal static bool inGame = false;
        internal static GameDTO globalDto = null;
        internal static GameDTO roomDto = null;
        internal static PlayerCredentialsDto runningGameDto = null;
        internal static List<ChampionDTO> champs = new List<ChampionDTO>();
        internal static string gameState = "NULL";
        public static string botLevelLimit;
        internal static string platform;
        internal static string lvlGroupType;
        public static bool groupSplitted = false;
        internal static bool acceptedGame = false;
        internal static bool gameStarted = false;
        internal static Mutex inviteMutex = new Mutex();
        internal static Thread observerThread = null;

        internal static AllSummonerData lastSummonerData = null;
        public static bool inInviteLobby = false;

        public static void restart()
        {
            var process = new System.Diagnostics.Process();
            process.StartInfo.FileName = "PvPNetClient.exe";
            process.StartInfo.UseShellExecute = false;

            string args = "";

            for (int i = 0; i < argsCopy.Length; ++i)
            {
                if (i == 0)
                {
                    args += argsCopy[i];
                }
                else
                {
                    if (argsCopy[i].Contains(" "))
                        args += " " + "\"" + argsCopy[i] + "\"";
                    else
                        args += " " + argsCopy[i];
                }
            }

            process.StartInfo.Arguments = args;
            process.Start();

            Thread.Sleep(500);

            System.Environment.Exit(0);
        }

        private static async void BuyExpBoost(String storeUrl)
        {
            using (HttpClient client = new HttpClient())
            using (HttpResponseMessage response = await client.GetAsync(storeUrl))
            using (HttpContent content = response.Content)
            {
                await content.ReadAsStringAsync();

                var pairs = new List<KeyValuePair<string, string>>
                    {
                        new KeyValuePair<string, string>("item_id", "boosts_2"),
                        new KeyValuePair<string, string>("currency_type", "rp"),
                        new KeyValuePair<string, string>("quantity", "1"),
                        new KeyValuePair<string, string>("rp", "260"),
                        new KeyValuePair<string, string>("ip", "null"),
                        new KeyValuePair<string, string>("duration_type", "PURCHASED"),
                        new KeyValuePair<string, string>("duration", "3"),
                    };

                var postData = new FormUrlEncodedContent(pairs);

                if (platform == "eune")
                    await client.PostAsync(new Uri("https://store.eun1.lol.riotgames.com/store/purchase/item"), postData);

                if (platform == "euw")
                    await client.PostAsync(new Uri("https://store.euw1.lol.riotgames.com/store/purchase/item"), postData);
            }
        }

        public static async Task<int> InvitePlayers()
        {
            //inviteMutex.WaitOne();

            //As soon as HOST or HOST2 hits LVL15 they will split the group in two pieces
            string _roomName = "";
            string _hostName = "";

            if (Client.botType == "HOST" || lvlGroupType == "ZERO")
            {
                _roomName = roomName;
                _hostName = hostName;
            }
            if (Client.botType == "HOST2" || lvlGroupType == "ONE")
            {
                _roomName = roomNameTwo;
                _hostName = hostNameTwo;
            }

            //Wait for INVITATIONLIST and invite all!
            //Console.WriteLine("Waiting for Invitationlist");

            string inviteFile = "";

            try
            {
                while (inviteFile.Split(':').Length != 5)
                {
                    try
                    {
                        inviteFile = System.IO.File.ReadAllText("Logs/Rooms/" + _hostName + ".invitationlist");
                    }
                    catch (Exception e)
                    {
                        //Console.WriteLine("READERROR: " + inviteFile);
                        inviteFile = "";
                    }

                    //Console.WriteLine("COUNT: " + inviteFile.Split(':').Length);
                    Thread.Sleep(5000);
                }

                foreach (string sumID in inviteFile.Split(':'))
                {
                    try
                    {
                        //Console.WriteLine("Inviting " + sumID);
                        await RiotCalls.Invite(Convert.ToInt32(sumID), "DEFAULT");
                    }
                    catch (Exception e)
                    {
                        //Console.WriteLine("INVITERROR: " + e.Message);
                    }
                }

                //await RiotCalls.Invite(23048384, "DEFAULT");
            }
            catch (Exception e)
            {
                Console.WriteLine("EXCEPTION:" + e);
            }

            //Console.WriteLine("Invitation done!");
            //inviteMutex.ReleaseMutex();
            return 1;
        }

        public static void StartClient(PlayerCredentialsDto dto)
        {
            System.IO.File.Delete("Logs/Rooms/" + hostName + ".roomID");

            //Start our ClientEmu with Params
            var process = new System.Diagnostics.Process();
            process.StartInfo = new ProcessStartInfo();
            process.StartInfo.FileName = "ClientEmu.exe";
            process.StartInfo.CreateNoWindow = true;
            process.StartInfo.WindowStyle = ProcessWindowStyle.Minimized;
            process.StartInfo.ErrorDialog = false;

            process.StartInfo.Arguments = dto.ServerIp + " " +
                dto.ServerPort + " " +
                dto.EncryptionKey + " " +
                dto.SummonerId + " " +
                Client.botType + " " +
                userName + " ";

            if (botLevelLimit == "LEVEL_30" && !groupSplitted) // CHANGE TO LEVEL_30
                process.StartInfo.Arguments += "LEVEL_10"; //Need this to fake dominion! CHANGE TO LEVEL_10
            //else if (botLevelLimit == "LEVEL_10")
            //    process.StartInfo.Arguments += "LEVEL_30";
            else
                process.StartInfo.Arguments += botLevelLimit;

            if (process.Start() == false)
                Console.WriteLine("Could no start ClientEmu.exe!");

            Console.WriteLine(DateTime.Now.ToString("HH:mm:ss tt") + " started");
            RiotCalls.SetClientReceivedGameMessage(globalDto.Id, "GAME_START_CLIENT");
            //RiotCalls.SetClientReceivedGameMessage(globalDto.Id, "GameClientConnectedToServer");
            //Client.RtmpConnection.MessageReceived -= ChampSelectHandler;

            acceptedGame = false;
            gameStarted = true;

            //try
            //{
            //    System.IO.File.Delete("Logs/Rooms/" + hostName + ".invitationlist");
            //}
            //catch (Exception e)
            //{ }

            //try
            //{
            //    System.IO.File.Delete("Logs/Rooms/" + hostNameTwo + ".invitationlist");
            //}
            //catch (Exception e)
            //{ }
        }

        private static void ChampSelectHandler(object sender, object message)
        {
            //Launching the Game
            if (((MessageReceivedEventArgs)message).Body.GetType() == typeof(PlayerCredentialsDto))
            {
                //Wait 5 Secs uh yeah!
                Thread.Sleep(5000);

                PlayerCredentialsDto dto = ((MessageReceivedEventArgs)message).Body as PlayerCredentialsDto;
                runningGameDto = dto;

                //if (Client.botType != "HOST" && lastSummonerData.SummonerLevelAndPoints.SummonerLevel >= 10 && botLevelLimit != "LEVEL_30")
                //{
                //    Console.WriteLine("Ignoring game due to we reached LVL_10 :))!");
                //    return;
                //}

                inInviteLobby = false;
                StartClient(dto);

                //observerThread = new Thread(CheckForReconnect);
                //observerThread.Start();

                //if (botLevelLimit == "LEVEL_30")
                //    Task.Run(async () => await CheckForReconnect());
            }
        }

        private static async void RoomMessageHandler(object sender, object message)
        {
            //Our group already finished?
            if (System.IO.File.Exists("Logs/" + hostName + ".gfinished"))
            {
                System.Environment.Exit(0);
            }

            //Console.WriteLine(groupSplitted);
            try
            {
                if (((MessageReceivedEventArgs)message).Body is RtmpSharp.IO.AsObject)
                {
                    if (!groupSplitted)
                        return;

                    //Convert object
                    RtmpSharp.IO.AsObject asnObject = ((MessageReceivedEventArgs)message).Body as AsObject;

                    if (asnObject.TypeName == "com.riotgames.platform.gameinvite.contract.InvitationRequest" && Client.botType == "BOT")
                    {
                        String InvitationState = asnObject["invitationState"] as String;

                        if (InvitationState != "ACTIVE")
                            return;

                        String InvitationId = asnObject["invitationId"] as String;

                        //Join them :)!
                        try
                        {
                            Console.WriteLine("Accepting Invitation");
                            await RiotCalls.Accept(InvitationId);
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine("INVITATION: " + e.Message);
                            return;
                        }

                        return;
                    }

                    if (Client.botType == "HOST" || Client.botType == "HOST2")
                    {
                        RtmpSharp.IO.AsObject lobbyStatus = asnObject;

                        //Check if 5 Invitations got sent
                        RtmpSharp.IO.AMF3.ArrayCollection invitees = lobbyStatus["invitees"] as RtmpSharp.IO.AMF3.ArrayCollection;

                        //if (invitees.Count != 5)
                        //{
                        //    await InvitePlayers();
                        //}

                        //Check if 5 Players are in lobby
                        RtmpSharp.IO.AMF3.ArrayCollection members = lobbyStatus["members"] as RtmpSharp.IO.AMF3.ArrayCollection;

                        if (members.Count == 5)
                        {
                            //Wait and attach to queue
                            Thread.Sleep(1500);

                            MatchMakerParams matchParams = new MatchMakerParams();
                            matchParams.Languages = null;
                            matchParams.BotDifficulty = "EASY";
                            matchParams.Team = new List<int>();

                            for (int i = 0; i < members.Count; ++i)
                            {
                                RtmpSharp.IO.AsObject memberObj = members[i] as RtmpSharp.IO.AsObject;
                                //Console.WriteLine(memberObj["summonerId"]);
                                matchParams.Team.Add(Convert.ToInt32(memberObj["summonerId"]));
                            }

                            matchParams.QueueIds = new int[] { 25 };
                            matchParams.InvitationId = lobbyStatus["invitationId"] as String;

                            await RiotCalls.AttachTeamToQueue(matchParams);

                            Console.WriteLine("Attached to Queue");
                        }
                    }
                }
                else if (message.GetType() == typeof(GameDTO) || ((MessageReceivedEventArgs)message).Body.GetType() == typeof(GameDTO))
                {
                    GameDTO dto = null;

                    try
                    {
                        if (message.GetType() == typeof(GameDTO))
                            dto = message as GameDTO;
                        else
                            dto = ((MessageReceivedEventArgs)message).Body as GameDTO;
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("Ignoring => Convert (object to GameDTO)");
                        return;
                    }

                    //Console.WriteLine(dto.GameState + ":" + groupSplitted);

                    //Game Popup?
                    if (dto.GameState == "JOINING_CHAMP_SELECT" && !acceptedGame)
                    {
                        try
                        {
                            await RiotCalls.AcceptPoppedGame(true);
                        }
                        catch (Exception e)
                        { }

                        acceptedGame = true;
                        return;
                    }

                    //Host left room? follow him!
                    //if (Client.botType == "BOT" && dto.GameState == "TEAM_SELECT")
                    //{
                    //    bool leaveRoom = false;

                    //    if (groupSplitted)
                    //    {
                    //        if (lvlGroupType == "ZERO")
                    //        {
                    //            if (dto.OwnerSummary.SummonerInternalName != hostName)
                    //                leaveRoom = true;
                    //        }

                    //        if (lvlGroupType == "ONE")
                    //        {
                    //            if (dto.OwnerSummary.SummonerInternalName != hostNameTwo)
                    //                leaveRoom = true;
                    //        }
                    //    }
                    //    else
                    //    {
                    //        if (dto.OwnerSummary.SummonerInternalName != hostName)
                    //            leaveRoom = true;
                    //    }

                    //    if (leaveRoom)
                    //    {
                    //        Console.WriteLine("Host left the game, follow him!");
                    //        restart();
                    //    }
                    //}

                    if (dto.GameState == "TEAM_SELECT" && !groupSplitted)
                    {
                        List<Participant> AllParticipants = new List<Participant>(dto.TeamOne.ToArray());
                        AllParticipants.AddRange(dto.TeamTwo);

                        //Console.WriteLine("Player joined the game! " + AllParticipants.Count.ToString() + "/10");
                        gameState = "TEAM_SELECT";

                        if (Client.botType == "HOST")
                        {
                            int playerRoomCount = 10;

                            if (AllParticipants.Count == playerRoomCount) // 10/5 Players joined?
                            {
                                Thread.Sleep(1500);

                                await RiotCalls.StartChampionSelection(dto.Id, dto.OptimisticLock);
                                Console.WriteLine("Room started!");
                                gameState = "CHAMP_SELECT";
                            }
                        }
                    }

                    //Something failed
                    if (dto.GameState.Contains("FAILED"))
                    {
                        Console.WriteLine("FATAL: " + dto.GameState);
                        restart();
                    }

                    //Roomstate changed
                    if (dto.GameState == "TEAM_SELECT" && gameState == "CHAMP_SELECT")
                    {
                        gameState = "TEAM_SELECT";
                        Console.WriteLine("Unknown player left the room!");
                        //restart();
                    }

                    //Selecting Champion etc.
                    if (dto.GameState == "CHAMP_SELECT" || dto.GameState == "PRE_CHAMP_SELECT")
                    {
                        inInviteLobby = false;
                        gameState = "CHAMP_SELECT";

                        if (!Client.selectedChampion)
                        {
                            Client.selectedChampion = true;
                            gameState = "CHAMP_SELECT";

                            //Signal to the server we are in champion select
                            try
                            {
                                await RiotCalls.SetClientReceivedGameMessage(dto.Id, "CHAMP_SELECT_CLIENT");
                                globalDto = dto;
                            }
                            catch (Exception e)
                            {
                                Console.WriteLine("Ignoring => " + e.Message);
                            }

                            try
                            {
                                await RiotCalls.ChampionSelectCompleted();
                                Console.WriteLine("Selected Champion and Locked");

                                //await RiotCalls.ChampionSelectCompleted();
                                //Console.WriteLine("[2]Selected Champion and Locked");
                            }
                            catch (Exception e)
                            {
                                Console.WriteLine("Locking in error: " + e.Message);
                            }
                        }
                    }

                    //Game ended
                    if (dto.GameState == "TERMINATED" || dto.GameState == "TERMINATED_IN_ERROR")
                    {
                        gameStarted = false;
                        acceptedGame = false;
                        gameState = "LOBBY";

                        AllSummonerData summonerData = null;
                        summonerData = await RiotCalls.GetAllSummonerDataByAccount(Client.PlayerSession.AccountSummary.AccountId);
                        lastSummonerData = summonerData;

                        if (Client.botType == "HOST")
                        {
                            try
                            {
                                System.IO.File.Delete("Logs/Rooms/" + hostName + ".roomID");
                            }
                            catch (Exception elol)
                            { }
                        }

                        Console.WriteLine(DateTime.Now.ToString("HH:mm:ss tt") + " end");
                        Console.WriteLine("Level " + summonerData.SummonerLevel.Level.ToString() + " " + summonerData.SummonerLevelAndPoints.ExpPoints.ToString() + "/" + summonerData.SummonerLevel.ExpToNextLevel.ToString());

                        //Remove GAMEEND file
                        try
                        {
                            System.IO.File.Delete("Logs/" + lastSummonerData.Summoner.SumId.ToString() + ".GAMEEND");
                        }
                        catch (Exception)
                        {
                        }

                        if (summonerData.SummonerLevel.Level == 3)
                        {
                            Console.WriteLine("Buying EXP Boost!");
                            BuyExpBoost(await RiotCalls.GetStoreUrl());
                        }

                        if ((botLevelLimit == "LEVEL_5" && summonerData.SummonerLevel.Level >= 5) || (botLevelLimit == "LEVEL_10" && summonerData.SummonerLevel.Level >= 10) || (botLevelLimit == "LEVEL_30" && summonerData.SummonerLevel.Level >= 30))
                        {
                            System.IO.File.WriteAllText(@"Logs/" + userName + ".finished", "1");
                        }

                        dynamic infoFile = Config.Load("Logs/AccountInfo/" + userName + ".info");

                        infoFile.Info.Level = summonerData.SummonerLevel.Level;
                        infoFile.Info.ExpBalance = summonerData.SummonerLevelAndPoints.ExpPoints.ToString() + "/" + summonerData.SummonerLevel.ExpToNextLevel.ToString();

                        infoFile.Save();
                        infoFile = null;

                        //Wait 5 Seconds for observerThreads!
                        //Thread.Sleep(5000);

                        //Make sure observerThread gets terminated
                        //try
                        //{
                        //    observerThread.Abort();
                        //}
                        //catch (Exception)
                        //{ }

                        doRoomStuff(); //Start Loop again
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("FAK:" + e.Message);
                restart();
            }
        }

        private static async void doRoomStuff()
        {
            Client.selectedChampion = false;

            if (!handlersRegistered)
            {
                Client.RtmpConnection.MessageReceived += RoomMessageHandler;
                Client.RtmpConnection.MessageReceived += ChampSelectHandler;
                handlersRegistered = true;
            }

            bool playLvL30Dominion = false;

            //Our group already finished?
            if (System.IO.File.Exists("Logs/" + hostName + ".gfinished"))
            {
                System.Environment.Exit(0);
            }

            if (gameStarted)
                return;

            //try
            //{
            //    await RiotCalls.Leave();
            //}
            //catch (Exception)
            //{ }

            //try
            //{
            //    await RiotCalls.QuitGame();
            //}
            //catch (Exception)
            //{ }

            //Check if we are in an active game?!!!

            //if (botLevelLimit == "LEVEL_30")
            //{
            //    try
            //    {
            //        var gameInfo = await RiotCalls.RetrieveInProgressGameInfo();

            //        if (gameInfo.PlayerCredentials != null)
            //        {
            //            groupSplitted = true;

            //            Console.WriteLine("Active Game Found: " + gameInfo.GameName);
            //            StartClient(gameInfo.PlayerCredentials);
            //            return;
            //        }
            //    }
            //    catch (Exception ingameException)
            //    {
            //        return;
            //    }
            //}

            if (botLevelLimit == "LEVEL_30")
            {
                //As soon as HOST or HOST2 hits LVL15 they will split the group in two pieces
                string _roomName = "";
                string _hostName = "";

                if (Client.botType == "HOST" || lvlGroupType == "ZERO")
                {
                    _roomName = roomName;
                    _hostName = hostName;
                }
                if (Client.botType == "HOST2" || lvlGroupType == "ONE")
                {
                    _roomName = roomNameTwo;
                    _hostName = hostNameTwo;
                }

                if (gameStarted)
                    return;

                //Wait for HOSTS!
                Console.Write("Waiting for Hosts! | ");

                while (System.IO.File.Exists("Logs/AccountInfo/" + hostName + ".info") == false || System.IO.File.Exists("Logs/AccountInfo/" + hostNameTwo + ".info") == false)
                    Thread.Sleep(10000);

                Console.WriteLine("OK");

                //Get HOST and HOST2 Info
                dynamic hostInfo = Config.Load("Logs/AccountInfo/" + hostName + ".info");
                dynamic hostTwoInfo = Config.Load("Logs/AccountInfo/" + hostNameTwo + ".info");

                //Did HOSt or HOST2 hit lvl15?
                if (hostInfo.Info.Level >= 10 || hostTwoInfo.Info.Level >= 10)
                {
                    groupSplitted = true;

                    //If we are HOST or HOST2
                    if (Client.botType == "HOST" || Client.botType == "HOST2")
                    {
                        CREATE_LOBBY:
                        try
                        {
                            //LobbyStatus lobbyStatus = await RiotCalls.CreateArrangedBotTeamLobby(33, "MEDIUM");
                            LobbyStatus lobbyStatus = await RiotCalls.CreateArrangedBotTeamLobby(25, "EASY");
                        }
                        catch (Exception e)
                        {
                            if (!e.Message.Contains("AlreadyMemberOfInvitation"))
                            {
                                Console.WriteLine("EXCEPTION WHILE CREATING LOBBY:" + e.Message);
                                Thread.Sleep(60000 * 5);
                                restart();
                                return;
                            }
                        }

                        inInviteLobby = true;
                        await InvitePlayers();
                    }
                    else //Normal Bot Instance here
                    {
                        bool noFile = false;
                        string inviteFile = "";

                        try
                        {
                            //Are we alread in the list?
                            inviteFile = System.IO.File.ReadAllText("Logs/Rooms/" + _hostName + ".invitationlist");
                        }
                        catch (Exception)
                        {
                            noFile = true;
                        }

                        if (!inviteFile.Contains(Convert.ToInt32(lastSummonerData.Summoner.SumId).ToString()))
                        {
                            Console.WriteLine("Writing sumID: " + Convert.ToInt32(lastSummonerData.Summoner.SumId).ToString());
                            System.IO.File.AppendAllText("Logs/Rooms/" + _hostName + ".invitationlist", Convert.ToInt32(lastSummonerData.Summoner.SumId).ToString() + ":");
                        }
                    }
                }
                else //So we wanna play Dominion then!
                {
                    playLvL30Dominion = true;
                }
            }

            if (botLevelLimit == "LEVEL_5" || botLevelLimit == "LEVEL_10" || playLvL30Dominion) //LEVEL_5 & LEVEL_10
            {
                try
                {
                    if (Client.botType == "HOST")
                    {
                        CreatePracticeRoom:
                        //Console.WriteLine("Creating Room...");
                        try
                        {
                            //Create Room
                            PracticeGameConfig config = new PracticeGameConfig();
                            config.GameTypeConfig = 1;
                            config.AllowSpectators = "DROPINONLY";
                            config.GamePassword = "1234";
                            config.PassbackUrl = null;
                            config.PassbackDataPacket = null;
                            config.GameName = roomName;
                            config.GameMode = "ODIN";
                            config.GameMap = GameMap.TheCrystalScar;
                            config.MaxNumPlayers = 10;

                            roomDto = await RiotCalls.CreatePracticeGame(config);

                            System.IO.File.WriteAllText("Logs/Rooms/" + hostName + ".roomID", roomDto.Id.ToString());
                            Console.WriteLine("Room created! : " + roomDto.Id.ToString());
                            gameState = roomDto.GameState;
                        }
                        catch (Exception e)
                        {
                            //Players[] already in Game - Error I guess, dunno "already" should be enough - TODO: Fix this later!
                            if (e.Message.Contains("already"))
                            {
                                try
                                {
                                    RiotCalls.QuitGame();
                                }
                                catch (Exception)
                                { }

                                try
                                {
                                    RiotCalls.Leave();
                                }
                                catch (Exception)
                                { }

                                Console.WriteLine("Players already in game | GUESSING SO | WAITING 1 Minute |" + e.Message);
                                System.Threading.Thread.Sleep(60000);
                            }
                            else
                            {
                                Console.WriteLine(e.Message);
                            }

                            System.Threading.Thread.Sleep(10000);
                            goto CreatePracticeRoom;
                        }
                    }
                    else if ((Client.botType == "BOT" || Client.botType == "HOST2"))
                    {
                        int joinTry = 0;
                        Console.WriteLine("Waiting for host...");

                        JoinRoom:
                        double gameId = 0;

                        while (gameId == 0)
                        {
                            try
                            {
                                gameId = Convert.ToDouble(System.IO.File.ReadAllText("Logs/Rooms/" + hostName + ".roomID"));
                            }
                            catch (Exception e)
                            {
                                gameId = 0;
                            }

                            System.Threading.Thread.Sleep(1000);
                        }

                        bool roomJoined = false;

                        try
                        {
                            Console.WriteLine("Joining Room... : " + gameId.ToString());
                            await RiotCalls.JoinGame(gameId, "1234");
                            roomJoined = true;
                            Console.WriteLine("Joined Room! = " + gameId.ToString());
                            gameState = "TEAM_SELECT";
                            return;
                        }
                        catch (Exception e)
                        {
                            if (e.Message.Contains("NotFound"))
                            {
                                try
                                {
                                    System.IO.File.Delete("Logs/Rooms/" + hostName + ".roomID");
                                }
                                catch (Exception fileDel)
                                { }

                                Console.WriteLine("Host room is no longer available!");
                            }
                            else
                            {
                                Console.WriteLine(e.Message);
                            }

                            roomJoined = false;
                        }

                        if (!roomJoined)
                        {
                            ++joinTry;

                            if (joinTry == 4)
                                restart();

                            if (!roomJoined)
                            {
                                //Console.WriteLine("Could not join game!");
                                System.Threading.Thread.Sleep(20000);

                                try
                                {
                                    RiotCalls.QuitGame();
                                }
                                catch (Exception)
                                {
                                }

                                goto JoinRoom;
                            }
                        }

                        gameState = "TEAM_SELECT";
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine("Exception! - Reconnecting...");
                    Client.RtmpConnection = null;
                    System.Threading.Thread.Sleep(1000);

                    restart();
                    //Console.WriteLine("Reconnecting...");
                    //Application.Restart();
                    //System.Environment.Exit(-1);
                }
            }
        }

        public static void OnInvocationException(object sender, Exception e)
        {
            Console.WriteLine("ONINVOCATIONEXCEPTION!");
            restart();
        }

        public static async void doMainStuff(string[] args)
        {
            // add this to your code before you make your web service call.
            System.Net.ServicePointManager.ServerCertificateValidationCallback +=
            delegate (object sender, X509Certificate cert, X509Chain chain, SslPolicyErrors sslError)
            {
                bool validationResult = true;
                return validationResult;
            };

            //Init parameters
            userName = args[1].ToLower();
            string password = args[2];
            Client.botType = args[3];
            roomName = args[4];
            roomNameTwo = args[5];
            hostName = args[6].ToLower();
            hostNameTwo = args[7].ToLower();
            botLevelLimit = args[8];
            platform = args[9];
            lvlGroupType = args[10];

            //So the ControlPanel knows who we are.
            Console.Title = userName;

            //Console.WriteLine(userName + ":" + password + " - " + roomName);

            handlersRegistered = false;
            inGame = false;
            gameState = "NULL";
            SerializationContext context;

            try
            {
                //Init struct for login
                AuthenticationCredentials newCredentials = new AuthenticationCredentials();

                newCredentials.Username = userName;
                newCredentials.Password = password;
                newCredentials.ClientVersion = "4.21.14_12_08_11_36";
                newCredentials.IpAddress = "10.20.52.244";
                newCredentials.Locale = "en_GB";
                newCredentials.Domain = "lolclient.lol.riotgames.com";
                newCredentials.OperatingSystem = "Windows XP";

                Console.Write("Log in! ... | ");

                //Login Queue here!
                try
                {
                    string loginQueueLink = "";

                    if (platform == "eune")
                        loginQueueLink = "https://lq.eun1.lol.riotgames.com/";

                    if (platform == "euw")
                        loginQueueLink = "https://lq.euw1.lol.riotgames.com/";

                    newCredentials.AuthToken = RiotCalls.GetAuthKey(userName, password, loginQueueLink);
                }
                catch (Exception e)
                {
                    if (e.Message.Contains("The remote name could not be resolved"))
                        Console.WriteLine("Please make sure you are connected the internet!");
                    else if (e.Message.Contains("(403) Forbidden"))
                        Console.WriteLine("Your username or password is incorrect!");
                    else
                    {
                        //if(botLevelLimit == "LEVEL_30" && e.Message.Contains("403"))
                        //{
                        //    Console.WriteLine("Got caught by LeaveBust - BANNED TEMPORARY! | Sleeping 1/2 Hour");
                        //    Thread.Sleep(3600000 / 2);
                        //    restart();
                        //}

                        Console.WriteLine("Unable to get Auth Key - " + e.Message);
                    }

                    System.Threading.Thread.Sleep(5000);
                    restart();
                }

                //Console.WriteLine(newCredentials.AuthToken);

                //Init Rtmp Connection here
                try
                {
                    string rtmpLink = "";

                    if (platform == "eune")
                        rtmpLink = "rtmps://prod.eun1.lol.riotgames.com:2099";
                    else if (platform == "euw")
                        rtmpLink = "rtmps://prod.euw1.lol.riotgames.com:2099";
                    else
                    {
                        Console.WriteLine("Unknown platform: " + platform);
                        Console.ReadKey();
                    }

                    context = RiotCalls.RegisterObjects();
                    Client.RtmpConnection = new RtmpClient(new Uri(rtmpLink), context, ObjectEncoding.Amf3);
                    Client.RtmpConnection.Disconnected += DisconnectedHandler;
                    Client.summonerName = userName.ToLower();
                    RiotCalls.OnInvocationError += OnInvocationException;

                    Random rnd = new Random();
                    int waitTime = rnd.Next(1, 61);
                    Console.Write("Waiting " + waitTime.ToString() + "s | ");
                    Thread.Sleep(waitTime * 1000);

                    await Client.RtmpConnection.ConnectAsync();
                }
                catch (Exception e)
                {
                    Console.WriteLine("FAILED");
                    System.Threading.Thread.Sleep(15000);
                    restart();
                }

                //Now login into the RTMP Server and register to message services
                //Console.WriteLine("[STEP 2]Loggin in...");
                Client.PlayerSession = await RiotCalls.Login(newCredentials);
                //await Client.RtmpConnection.SubscribeAsync("my-rtmps", "messagingDestination", "bc", "bc-" + Client.PlayerSession.AccountSummary.AccountId.ToString());
                //await Client.RtmpConnection.SubscribeAsync("my-rtmps", "messagingDestination", "gn-" + Client.PlayerSession.AccountSummary.AccountId.ToString(), "gn-" + Client.PlayerSession.AccountSummary.AccountId.ToString());
                //await Client.RtmpConnection.SubscribeAsync("my-rtmps", "messagingDestination", "cn-" + Client.PlayerSession.AccountSummary.AccountId.ToString(), "cn-" + Client.PlayerSession.AccountSummary.AccountId.ToString());
                bool loginSuccess = await Client.RtmpConnection.LoginAsync(userName, Client.PlayerSession.Token);
                //Console.WriteLine("Full Access " + loginSuccess.ToString());

                //Start heartbeat timer here
                try
                {
                    Client.StartHeartbeat();
                }
                catch (Exception e)
                { }
            }
            catch (Exception e)
            {
                if (e.Message.Contains("Wrong client version"))
                {
                    Console.WriteLine("NEW LOL UPDATE, WAIT FOR UPDATE FROM OMDIHAR! | " + e.Message);
                    Console.ReadKey();
                    System.Environment.Exit(0);
                }

                Console.WriteLine("FAILED : " + e.Message);

                System.Threading.Thread.Sleep(5000);
                restart();
            }

            //Console.WriteLine("LOGIN | UH YEAH => " + Client.PlayerSession.AccountSummary.AccountId.ToString());
            Console.WriteLine("SUCCESS!");

            try
            {
                Client.accId = Client.PlayerSession.AccountSummary.AccountId;
            }
            catch (Exception e)
            {
                Console.WriteLine("Could not assign accId :" + e.Message);
            }

            try
            {
                await RiotCalls.CreateDefaultSummoner(userName);
            }
            catch (Exception e)
            {
                Console.WriteLine("CreateDefaultSummoner: " + userName);
            }
            //Console.WriteLine("Creating summoner...");

            AllSummonerData summonerData = await RiotCalls.GetAllSummonerDataByAccount(Client.accId);

            try
            {
                //Console.WriteLine("Setting profile icon...");
                RiotCalls.UpdateProfileIconId(28);
            }
            catch (Exception e)
            {
            }

            try
            {
                //Console.WriteLine("Processing Elo...");
                await RiotCalls.ProcessEloQuestionaire("BEGINNER");
            }
            catch (Exception e)
            {
            }

            try
            {
                //Console.WriteLine("Skip Tutorial...");
                await RiotCalls.SaveSeenTutorialFlag();
            }
            catch (Exception e)
            {
            }

            try
            {
                Console.WriteLine("Lvl" + summonerData.SummonerLevel.Level.ToString() + " Exp:" + summonerData.SummonerLevelAndPoints.ExpPoints.ToString() + "/" + summonerData.SummonerLevel.ExpToNextLevel.ToString());
            }
            catch (Exception e)
            {
                Console.WriteLine("Could not display LEVEL : " + e.Message);

                for (int i = 0; i < 10; ++i)
                    Console.WriteLine("ACCOUNT BUGGED!!!!!!!!!");

                Console.ReadKey();
            }

            if (summonerData.SummonerLevel.Level == 3)
            {
                Console.WriteLine("Buying EXP Boost!");
                BuyExpBoost(await RiotCalls.GetStoreUrl());
            }

            if ((botLevelLimit == "LEVEL_5" && summonerData.SummonerLevel.Level >= 5) || (botLevelLimit == "LEVEL_10" && summonerData.SummonerLevel.Level >= 10) || (botLevelLimit == "LEVEL_30" && summonerData.SummonerLevel.Level >= 30))
            {
                System.IO.File.WriteAllText(@"Logs/" + userName + ".finished", "1");
            }

            try
            {
                dynamic infoFile = Config.Load("Logs/AccountInfo/" + userName + ".info");

                infoFile.Info.Level = summonerData.SummonerLevel.Level;
                infoFile.Info.ExpBalance = summonerData.SummonerLevelAndPoints.ExpPoints.ToString() + "/" + summonerData.SummonerLevel.ExpToNextLevel.ToString();

                infoFile.Save();
            }
            catch (Exception e)
            {
                Console.WriteLine("Could not prepare INFO File: " + e.Message);
            }

            gameState = "LOBBY";
            lastSummonerData = summonerData;

            doRoomStuff();
        }

        private static async void onProcessExit(object sender, EventArgs e)
        {
            await Client.RtmpConnection.SubscribeAsync("my-rtmps", "messagingDestination", "bc", "bc-" + Client.PlayerSession.AccountSummary.AccountId.ToString());
            await Client.RtmpConnection.SubscribeAsync("my-rtmps", "messagingDestination", "gn-" + Client.PlayerSession.AccountSummary.AccountId.ToString(), "gn-" + Client.PlayerSession.AccountSummary.AccountId.ToString());
            await Client.RtmpConnection.SubscribeAsync("my-rtmps", "messagingDestination", "cn-" + Client.PlayerSession.AccountSummary.AccountId.ToString(), "cn-" + Client.PlayerSession.AccountSummary.AccountId.ToString());
            await Client.RtmpConnection.LogoutAsync();
        }

        private static void UnhandledExceptionHandler(object sender, UnhandledExceptionEventArgs e)
        {
            //Our group already finished?
            if (System.IO.File.Exists("Logs/" + hostName + ".gfinished"))
            {
                System.Environment.Exit(0);
            }

            Console.WriteLine("Unhandled Exception caught!" + ((Exception)e.ExceptionObject).Message);
            Thread.Sleep(5000);
            restart();
        }

        private static void ThreadUnhandledExceptionHandler(object sender, ThreadExceptionEventArgs e)
        {
            //Our group already finished?
            if (System.IO.File.Exists("Logs/" + hostName + ".gfinished"))
            {
                System.Environment.Exit(0);
            }

            Console.WriteLine("Unhandled Exception caught!");
            Thread.Sleep(5000);
            restart();
        }

        private static void DisconnectedHandler(object o, EventArgs e)
        {
            //Our group already finished?
            if (System.IO.File.Exists("Logs/" + hostName + ".gfinished"))
            {
                System.Environment.Exit(0);
            }

            Console.WriteLine("DISCONNECT!");
            Thread.Sleep(5000);

            restart();
        }

        private static void Main(string[] args)
        {
            Application.ThreadException += ThreadUnhandledExceptionHandler;
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(UnhandledExceptionHandler);
            AppDomain.CurrentDomain.ProcessExit += new EventHandler(onProcessExit);

            //Console.ReadKey(true)
            argsCopy = new string[args.Length];

            for (int i = 0; i < args.Length; ++i)
                argsCopy[i] = args[i];

            doMainStuff(args);

            Thread.Sleep(Timeout.Infinite);
        }
    }
}