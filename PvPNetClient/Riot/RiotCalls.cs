﻿using com.riotgames.platform.gameinvite.contract;
using LegendaryClient.Logic.Riot.Kudos;
using LegendaryClient.Logic.Riot.Leagues;
using LegendaryClient.Logic.Riot.Platform;
using LegendaryClient.Logic.Riot.Team;
using RtmpSharp.IO;
using RtmpSharp.Messaging;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Web.Script.Serialization;

namespace LegendaryClient.Logic.Riot
{
    public class RiotCalls
    {
        public delegate void OnInvocationErrorHandler(object sender, Exception error);

        public static event OnInvocationErrorHandler OnInvocationError;

        /// <summary>
        /// Login to Riot's servers.
        /// </summary>
        /// <param name="Credentials">The credentials for the user</param>
        /// <returns>Session information for the user</returns>
        public static Task<Session> Login(AuthenticationCredentials Credentials)
        {
            return InvokeAsync<Session>("loginService", "login", Credentials);
        }

        /// <summary>
        /// Heartbeat to send every 2 minutes.
        /// </summary>
        /// <param name="AccountId">The users id</param>
        /// <param name="SessionToken">The token for the user</param>
        /// <param name="HeartbeatCount">The current amount that heartbeat has been sent</param>
        /// <param name="CurrentTime">The current time in GMT-0700 in format ddd MMM d yyyy HH:mm:ss</param>
        public static Task<string> PerformLCDSHeartBeat(Int32 AccountId, String SessionToken, Int32 HeartbeatCount, String CurrentTime)
        {
            return InvokeAsync<string>("loginService", "performLCDSHeartBeat", AccountId, SessionToken, HeartbeatCount, CurrentTime);
        }

        /// <summary>
        /// Gets the store url with token information for the current user.
        /// </summary>
        /// <returns>Returns the store URL</returns>
        public static Task<String> GetStoreUrl()
        {
            return InvokeAsync<String>("loginService", "getStoreUrl");
        }

        /// <summary>
        /// Gets the state for the current account
        /// </summary>
        /// <returns>Return the accounts state</returns>
        public static Task<String> GetAccountState()
        {
            return InvokeAsync<String>("accountService", "getAccountStateForCurrentSession");
        }

        public static Task<object> SelectBotChampion(Int32 champID, BotParticipant bot)
        {
            return InvokeAsync<object>("gameService", "selectBotChampion", champID, bot);
        }

        public static Task<LobbyStatus> CreateArrangedBotTeamLobby(Int32 unkID, String botDif)
        {
            return InvokeAsync<LobbyStatus>("lcdsGameInvitationService", "createArrangedBotTeamLobby", unkID, botDif);
        }

        public static Task<object> Invite(double summonerID, string inviteType)
        {
            return InvokeAsync<object>("lcdsGameInvitationService", "invite", summonerID, inviteType);
        }

        public static Task<LobbyStatus> Accept(String InvitationId)
        {
            return InvokeAsync<LobbyStatus>("lcdsGameInvitationService", "accept", InvitationId);
        }

        public static Task<object> Leave()
        {
            return InvokeAsync<object>("lcdsGameInvitationService", "leave");
        }

        /// <summary>
        /// Gets the login packet for the user with all the information for the user.
        /// </summary>
        /// <returns>Returns the login data packet</returns>
        public static Task<LoginDataPacket> GetLoginDataPacketForUser()
        {
            return InvokeAsync<LoginDataPacket>("clientFacadeService", "getLoginDataPacketForUser");
        }

        public static Task<object> SaveSeenTutorialFlag()
        {
            return InvokeAsync<object>("summonerService", "saveSeenTutorialFlag");
        }

        /// <summary>
        /// Call kudos (get information or send a kudos).
        /// </summary>
        /// <param name="JSONInformation"></param>
        /// <returns>Json Data about kudos</returns>
        public static Task<LcdsResponseString> CallKudos(String JSONInformation)
        {
            return InvokeAsync<LcdsResponseString>("clientFacadeService", "callKudos", JSONInformation);
        }

        /// <summary>
        /// Get the queues that are currently enabled.
        /// </summary>
        /// <returns>Returns an array of queues that are enabled</returns>
        public static Task<GameQueueConfig[]> GetAvailableQueues()
        {
            return InvokeAsync<GameQueueConfig[]>("matchmakerService", "getAvailableQueues");
        }

        /// <summary>
        /// Get the current IP & EXP Boosts for the user.
        /// </summary>
        /// <returns>Returns the active boosts for the user</returns>
        public static Task<SummonerActiveBoostsDTO> GetSumonerActiveBoosts()
        {
            return InvokeAsync<SummonerActiveBoostsDTO>("inventoryService", "getSumonerActiveBoosts");
        }

        /// <summary>
        /// Get the current champions for the user.
        /// </summary>
        /// <returns>Returns an array of champions</returns>
        public static Task<ChampionDTO[]> GetAvailableChampions()
        {
            return InvokeAsync<ChampionDTO[]>("inventoryService", "getAvailableChampions");
        }

        /// <summary>
        /// Get the runes the user owns.
        /// </summary>
        /// <param name="SummonerId">The summoner ID for the user</param>
        /// <returns>Returns the inventory for the user</returns>
        public static Task<SummonerRuneInventory> GetSummonerRuneInventory(Double SummonerId)
        {
            return InvokeAsync<SummonerRuneInventory>("summonerRuneService", "getSummonerRuneInventory", SummonerId);
        }

        /// <summary>
        /// Get the current Mastery Book for the user.
        /// </summary>
        /// <param name="SummonerId">The summoner ID for the user</param>
        /// <returns>Returns the mastery books for the user</returns>
        public static Task<MasteryBookDTO> GetMasteryBook(Double SummonerId)
        {
            return InvokeAsync<MasteryBookDTO>("summonerRuneService", "getSummonerRuneInventory", SummonerId);
        }

        /// <summary>
        /// Gets the runes for a user
        /// </summary>
        /// <param name="SummonerId">The summoner ID for the user</param>
        /// <returns>Returns the rune pages for a user</returns>
        public static Task<SpellBookDTO> GetSpellBook(Double SummonerId)
        {
            return InvokeAsync<SpellBookDTO>("spellBookService", "getSpellBook", SummonerId);
        }

        /// <summary>
        /// Gets the league positions for the user
        /// </summary>
        /// <returns>Returns the league positions for a user</returns>
        public static Task<SummonerLeagueItemsDTO> GetMyLeaguePositions()
        {
            return InvokeAsync<SummonerLeagueItemsDTO>("leaguesServiceProxy", "getMyLeaguePositions");
        }

        public static Task<SummonerLeagueItemsDTO> GetMyLeaguePositionsAndProgress()
        {
            return InvokeAsync<SummonerLeagueItemsDTO>("leaguesServiceProxy", "getMyLeaguePositionsAndProgress");
        }

        /// <summary>
        /// Gets the top 50 players for a queue type
        /// </summary>
        /// <param name="queueType">Queue type</param>
        /// <returns>Returns the top 50 players league info</returns>
        public static Task<LeagueListDTO> GetChallengerLeague(String queueType)
        {
            return InvokeAsync<LeagueListDTO>("leaguesServiceProxy", "getChallengerLeague", queueType);
        }

        /// <summary>
        /// Gets the current leagues for a user's tier (e.g Gold)
        /// </summary>
        /// <returns>Returns the leagues for a user</returns>
        public static Task<SummonerLeaguesDTO> GetAllMyLeagues()
        {
            return InvokeAsync<SummonerLeaguesDTO>("leaguesServiceProxy", "getAllMyLeagues");
        }

        /// <summary>
        /// Gets the league for a team
        /// </summary>
        /// <param name="TeamName">The team name</param>
        /// <returns>Returns the league information for a team</returns>
        public static Task<SummonerLeaguesDTO> GetLeaguesForTeam(String TeamName)
        {
            return InvokeAsync<SummonerLeaguesDTO>("leaguesServiceProxy", "getLeaguesForTeam", TeamName);
        }

        /// <summary>
        /// Get the leagues for a player
        /// </summary>
        /// <param name="SummonerId">The summoner id of the player</param>
        /// <returns>Returns the league information for a team</returns>
        public static Task<SummonerLeaguesDTO> GetAllLeaguesForPlayer(Double SummonerId)
        {
            return InvokeAsync<SummonerLeaguesDTO>("leaguesServiceProxy", "getAllLeaguesForPlayer", SummonerId);
        }

        /// <summary>
        ///
        /// </summary>
        public static Task<PlayerDTO> CreatePlayer()
        {
            return InvokeAsync<PlayerDTO>("summonerTeamService", "createPlayer");
        }

        public static Task<AllSummonerData> CreateDefaultSummoner(string summonerName)
        {
            return InvokeAsync<AllSummonerData>("summonerService", "createDefaultSummoner", summonerName);
        }

        /// <summary>
        /// Find a team by the TeamId
        /// </summary>
        /// <param name="TeamId">The team Id</param>
        /// <returns>Returns the information for a team</returns>
        public static Task<TeamDTO> FindTeamById(TeamId TeamId)
        {
            return InvokeAsync<TeamDTO>("summonerTeamService", "findTeamById", TeamId);
        }

        public static Task<TeamDTO> ProcessEloQuestionaire(string elo)
        {
            return InvokeAsync<TeamDTO>("playerStatsService", "processEloQuestionaire", elo);
        }

        /// <summary>
        /// Find a team by name
        /// </summary>
        /// <param name="TeamName">The team name</param>
        /// <returns>Returns the information for a team</returns>
        public static Task<TeamDTO> FindTeamByName(String TeamName)
        {
            return InvokeAsync<TeamDTO>("summonerTeamService", "findTeamByName", TeamName);
        }

        /// <summary>
        /// Disbands a team
        /// </summary>
        /// <param name="TeamId">The team Id</param>
        public static Task<object> DisbandTeam(TeamId TeamId)
        {
            return InvokeAsync<object>("summonerTeamService", "disbandTeam", TeamId);
        }

        /// <summary>
        /// Checks if a name is available
        /// </summary>
        /// <param name="TeamName">The name that you want to validate</param>
        /// <returns>Returns a boolean as the result</returns>
        public static Task<Boolean> IsTeamNameValidAndAvailable(String TeamName)
        {
            return InvokeAsync<Boolean>("summonerTeamService", "isNameValidAndAvailable", TeamName);
        }

        /// <summary>
        /// Checks if a tag is available
        /// </summary>
        /// <param name="TeamName">The tag that you want to validate</param>
        /// <returns>Returns a boolean as the result</returns>
        public static Task<Boolean> IsTeamTagValidAndAvailable(String TagName)
        {
            return InvokeAsync<Boolean>("summonerTeamService", "isTagValidAndAvailable", TagName);
        }

        /// <summary>
        /// Creates a ranked team if the name and tag is valid
        /// </summary>
        /// <param name="TeamName">The team name</param>
        /// <param name="TagName">The tag name</param>
        /// <returns>Returns the information for a team</returns>
        public static Task<TeamDTO> CreateTeam(String TeamName, String TagName)
        {
            return InvokeAsync<TeamDTO>("summonerTeamService", "createTeam", TeamName, TagName);
        }

        /// <summary>
        /// Invites a player to a ranked team
        /// </summary>
        /// <param name="SummonerId">The summoner id of the player you want to invite</param>
        /// <param name="TeamId">The team id</param>
        /// <returns>Returns the information for a team</returns>
        public static Task<TeamDTO> TeamInvitePlayer(Double SummonerId, TeamId TeamId)
        {
            return InvokeAsync<TeamDTO>("summonerTeamService", "invitePlayer", SummonerId, TeamId);
        }

        /// <summary>
        /// Kicks a player from a ranked team
        /// </summary>
        /// <param name="SummonerId">The summoner id of the player you want to kick</param>
        /// <param name="TeamId">The team id</param>
        /// <returns>Returns the information for a team</returns>
        public static Task<TeamDTO> KickPlayer(Double SummonerId, TeamId TeamId)
        {
            return InvokeAsync<TeamDTO>("summonerTeamService", "kickPlayer", SummonerId, TeamId);
        }

        /// <summary>
        /// Finds a player by Summoner Id
        /// </summary>
        /// <param name="SummonerId">The summoner id</param>
        /// <returns>Returns the information for a player</returns>
        public static Task<PlayerDTO> FindPlayer(Double SummonerId)
        {
            return InvokeAsync<PlayerDTO>("summonerTeamService", "findPlayer", SummonerId);
        }

        /// <summary>
        /// Gets summoner data by account id
        /// </summary>
        /// <param name="AccountId">The account id</param>
        /// <returns>Returns all the summoner data for an account</returns>
        public static Task<AllSummonerData> GetAllSummonerDataByAccount(Double AccountId)
        {
            return InvokeAsync<AllSummonerData>("summonerService", "getAllSummonerDataByAccount", AccountId);
        }

        public static Task<AllSummonerData> GetAllSummonerData(String summonerName)
        {
            return InvokeAsync<AllSummonerData>("summonerService", "getAllSummonerData", summonerName);
        }

        /// <summary>
        /// Gets summoner by name
        /// </summary>
        /// <param name="SummonerName">The name of the summoner</param>
        /// <returns>Returns the summoner</returns>
        public static Task<PublicSummoner> GetSummonerByName(String SummonerName)
        {
            return InvokeAsync<PublicSummoner>("summonerService", "getSummonerByName", SummonerName);
        }

        /// <summary>
        /// Gets the public summoner data by account id
        /// </summary>
        /// <param name="AccountId">The account id</param>
        /// <returns>Returns all the public summoner data for an account</returns>
        public static Task<AllPublicSummonerDataDTO> GetAllPublicSummonerDataByAccount(Double AccountId)
        {
            return InvokeAsync<AllPublicSummonerDataDTO>("summonerService", "getAllPublicSummonerDataByAccount", AccountId);
        }

        /// <summary>
        /// Gets the summoner internal name of a summoner
        /// </summary>
        /// <param name="SummonerName">The summoner name</param>
        /// <returns>Returns a summoners internal name</returns>
        public static Task<String> GetSummonerInternalNameByName(String SummonerName)
        {
            return InvokeAsync<String>("summonerService", "getSummonerInternalNameByName", SummonerName);
        }

        /// <summary>
        /// Updates the profile icon for the user
        /// </summary>
        /// <param name="IconId">The icon id</param>
        public static Task<object> UpdateProfileIconId(Int32 IconId)
        {
            return InvokeAsync<object>("summonerService", "updateProfileIconId", IconId);
        }

        /// <summary>
        /// Get the summoner names for an array of Summoner IDs.
        /// </summary>
        /// <param name="SummonerIds">Array of Summoner IDs</param>
        /// <returns>Returns an array of Summoner Names</returns>
        public static Task<String[]> GetSummonerNames(Double[] SummonerIds)
        {
            return InvokeAsync<String[]>("summonerService", "getSummonerNames", SummonerIds);
        }

        /// <summary>
        /// Gets the player reroll balance
        /// </summary>
        /// <returns>Returns the reroll balance for the player</returns>
        public static Task<PointSummary> GetPointsBalance()
        {
            return InvokeAsync<PointSummary>("lcdsRerollService", "getPointsBalance");
        }

        /// <summary>
        /// Attempts to reroll the champion. Only works in AllRandomPickStrategy
        /// </summary>
        /// <returns>Returns the amount of rolls left for the player</returns>
        public static Task<RollResult> Roll()
        {
            return InvokeAsync<RollResult>("lcdsRerollService", "roll");
        }

        /// <summary>
        /// Gets the players overall stats
        /// </summary>
        /// <param name="AccountId">The account id</param>
        /// <param name="Season">The season you want to retrieve stats from</param>
        /// <returns>Returns the player stats for a season</returns>
        public static Task<PlayerLifetimeStats> RetrievePlayerStatsByAccountId(Double AccountId, String Season)
        {
            return InvokeAsync<PlayerLifetimeStats>("playerStatsService", "retrievePlayerStatsByAccountId", AccountId, Season);
        }

        /// <summary>
        /// Gets the top 3 played champions for a player
        /// </summary>
        /// <param name="AccountId">The account id</param>
        /// <param name="GameMode">The game mode</param>
        /// <returns>Returns an array of the top 3 champions</returns>
        public static Task<ChampionStatInfo[]> RetrieveTopPlayedChampions(Double AccountId, String GameMode)
        {
            return InvokeAsync<ChampionStatInfo[]>("playerStatsService", "retrieveTopPlayedChampions", AccountId, GameMode);
        }

        /// <summary>
        /// Gets the aggregated stats of a players ranked games
        /// </summary>
        /// <param name="SummonerId">The summoner id of a player</param>
        /// <param name="GameMode">The game mode requested</param>
        /// <param name="Season">The season you want to retrieve stats from</param>
        /// <returns>Returns the aggregated stats requested</returns>
        public static Task<AggregatedStats> GetAggregatedStats(Double SummonerId, String GameMode, String Season)
        {
            return InvokeAsync<AggregatedStats>("playerStatsService", "getAggregatedStats", SummonerId, GameMode, Season);
        }

        /// <summary>
        /// Gets the top 10 recent games for a player
        /// </summary>
        /// <param name="AccountId">The account id of a player</param>
        /// <returns>Returns the recent games for a player</returns>
        public static Task<RecentGames> GetRecentGames(Double AccountId)
        {
            return InvokeAsync<RecentGames>("playerStatsService", "getRecentGames", AccountId);
        }

        /// <summary>
        /// Gets the aggregated stats for a team for all game modes
        /// </summary>
        /// <param name="TeamId">The team id</param>
        /// <returns>Returns an array </returns>
        public static Task<TeamAggregatedStatsDTO[]> GetTeamAggregatedStats(TeamId TeamId)
        {
            return InvokeAsync<TeamAggregatedStatsDTO[]>("playerStatsService", "getTeamAggregatedStats", TeamId);
        }

        /// <summary>
        /// Gets the end of game stats for a team for any game
        /// </summary>
        /// <param name="TeamId">The team id</param>
        /// <param name="GameId">The game id</param>
        /// <returns>Returns the end of game stats for a game</returns>
        public static Task<EndOfGameStats> GetTeamEndOfGameStats(TeamId TeamId, Double GameId)
        {
            return InvokeAsync<EndOfGameStats>("playerStatsService", "getTeamEndOfGameStats", TeamId, GameId);
        }

        /// <summary>
        /// Attaches to a queue
        /// </summary>
        /// <param name="MatchMakerParams">The parameters for the queue</param>
        /// <returns>Returns a notification to tell you if it was successful</returns>
        public static Task<SearchingForMatchNotification> AttachToQueue(MatchMakerParams MatchMakerParams)
        {
            return InvokeAsync<SearchingForMatchNotification>("matchmakerService", "attachToQueue", MatchMakerParams);
        }

        /// <summary>
        /// Attemps to leave a queue
        /// </summary>
        /// <param name="SummonerId">The users summoner id</param>
        /// <returns>If successfully cancelled returns true, otherwise champion select about to start</returns>
        public static Task<Boolean> CancelFromQueueIfPossible(Double SummonerId)
        {
            return InvokeAsync<Boolean>("matchmakerService", "cancelFromQueueIfPossible", SummonerId);
        }

        /// <summary>
        /// Accepts an invite to a matchmaking game
        /// </summary>
        /// <param name="InviteId">The invite id</param>
        public static Task<object> AcceptInviteForMatchmakingGame(String InviteId)
        {
            return InvokeAsync<object>("matchmakerService", "acceptInviteForMatchmakingGame", InviteId);
        }

        /// <summary>
        /// Attaches a premade team to a queue
        /// </summary>
        /// <param name="MatchMakerParams">The parameters for the queue</param>
        /// <returns>Returns a notification to tell you if it was successful</returns>
        public static Task<SearchingForMatchNotification> AttachTeamToQueue(MatchMakerParams matchMakerParams)
        {
            return InvokeAsync<SearchingForMatchNotification>("matchmakerService", "attachTeamToQueue", matchMakerParams);
        }

        /// <summary>
        /// Gets all the practice games
        /// </summary>
        /// <returns>Returns an array of practice games</returns>
        public static Task<PracticeGameSearchResult[]> ListAllPracticeGames()
        {
            return InvokeAsync<PracticeGameSearchResult[]>("gameService", "listAllPracticeGames");
        }

        /// <summary>
        /// Joins a game
        /// </summary>
        /// <param name="GameId">The game id the user wants to join</param>
        public static Task<object> JoinGame(Double GameId)
        {
            return InvokeAsync<object>("gameService", "joinGame", GameId, null);
        }

        /// <summary>
        /// Joins a private game
        /// </summary>
        /// <param name="GameId">The game id the user wants to join</param>
        /// <param name="Password">The password of the game</param>
        /// <returns></returns>
        public static Task<object> JoinGame(Double GameId, String Password)
        {
            return InvokeAsync<object>("gameService", "joinGame", GameId, Password);
        }

        public static Task<object> ObserveGame(Double GameId)
        {
            return InvokeAsync<object>("gameService", "observeGame", GameId, null);
        }

        public static Task<object> ObserveGame(Double GameId, String Password)
        {
            return InvokeAsync<object>("gameService", "observeGame", GameId, Password);
        }

        /// <summary>
        /// Switches the teams in a custom game
        /// </summary>
        /// <param name="GameId">The game id</param>
        /// <returns>Returns true if successful</returns>
        public static Task<Boolean> SwitchTeams(Double GameId)
        {
            return InvokeAsync<Boolean>("gameService", "switchTeams", GameId);
        }

        /// <summary>
        /// Switches from a player to spectator
        /// </summary>
        /// <param name="GameId">The game id</param>
        /// <returns>Returns true if successful</returns>
        public static Task<Boolean> SwitchPlayerToObserver(Double GameId)
        {
            return InvokeAsync<Boolean>("gameService", "switchPlayerToObserver", GameId);
        }

        /// <summary>
        /// Switches from a spectator to player
        /// </summary>
        /// <param name="GameId">The game id</param>
        /// <returns>Returns true if successful</returns>
        public static Task<Boolean> SwitchObserverToPlayer(Double GameId, Int32 Team)
        {
            return InvokeAsync<Boolean>("gameService", "switchObserverToPlayer", GameId, Team);
        }

        /// <summary>
        /// Quits from the current game
        /// </summary>
        public static Task<object> QuitGame()
        {
            return InvokeAsync<object>("gameService", "quitGame");
        }

        /// <summary>
        /// Creates a practice game.
        /// </summary>
        /// <param name="Config">The configuration for the practice game</param>
        /// <returns>Returns a GameDTO if successfully created, otherwise null</returns>
        public static Task<GameDTO> CreatePracticeGame(PracticeGameConfig Config)
        {
            return InvokeAsync<GameDTO>("gameService", "createPracticeGame", Config);
        }

        /// <summary>
        /// Starts champion selection for a custom game
        /// </summary>
        /// <param name="GameId">The game id</param>
        /// <param name="OptomisticLock">The optomistic lock (provided by GameDTO)</param>
        /// <returns>Returns a StartChampSelectDTO</returns>
        public static Task<StartChampSelectDTO> StartChampionSelection(Double GameId, Double OptomisticLock)
        {
            return InvokeAsync<StartChampSelectDTO>("gameService", "startChampionSelection", GameId, OptomisticLock);
        }

        /// <summary>
        /// Send a message to the server
        /// </summary>
        /// <param name="GameId">The game id</param>
        /// <param name="Argument">The argument to be passed</param>
        public static Task<object> SetClientReceivedGameMessage(Double GameId, String Argument)
        {
            return InvokeAsync<object>("gameService", "setClientReceivedGameMessage", GameId, Argument);
        }

        /// <summary>
        /// Gets the latest GameDTO for a game
        /// </summary>
        /// <param name="GameId">The game id</param>
        /// <param name="GameState">The current game state</param>
        /// <param name="PickTurn">The current pick turn</param>
        /// <returns>Returns the latest GameDTO</returns>
        public static Task<GameDTO> GetLatestGameTimerState(Double GameId, String GameState, Int32 PickTurn)
        {
            return InvokeAsync<GameDTO>("gameService", "getLatestGameTimerState", GameId, GameState, PickTurn);
        }

        /// <summary>
        /// Selects the spells for a player for the current game
        /// </summary>
        /// <param name="SpellOneId">The spell id for the first spell</param>
        /// <param name="SpellTwoId">The spell id for the second spell</param>
        public static Task<object> SelectSpells(Int32 SpellOneId, Int32 SpellTwoId)
        {
            return InvokeAsync<object>("gameService", "selectSpells", SpellOneId, SpellTwoId);
        }

        /// <summary>
        /// Selects a rune page for use
        /// </summary>
        /// <param name="SpellbookPage">The spellbook page the player wants to use</param>
        /// <returns>The selected spellbook page</returns>
        public static Task<SpellBookPageDTO> SelectDefaultSpellBookPage(SpellBookPageDTO SpellbookPage)
        {
            return InvokeAsync<SpellBookPageDTO>("spellBookService", "selectDefaultSpellBookPage", SpellbookPage);
        }

        /// <summary>
        /// Saves the players spellbook
        /// </summary>
        /// <param name="Spellbook">The players SpellBookDTO</param>
        public static Task<object> SaveSpellBook(SpellBookDTO Spellbook)
        {
            return InvokeAsync<object>("spellBookService", "saveSpellBook", Spellbook);
        }

        /// <summary>
        /// Selects a champion for use
        /// </summary>
        /// <param name="ChampionId">The selected champion id</param>
        public static Task<object> SelectChampion(Int32 ChampionId)
        {
            return InvokeAsync<object>("gameService", "selectChampion", ChampionId);
        }

        /// <summary>
        /// Selects a champion skin for a champion
        /// </summary>
        /// <param name="ChampionId">The selected champion id</param>
        /// <param name="SkinId">The selected champion skin</param>
        public static Task<object> SelectChampionSkin(Int32 ChampionId, Int32 SkinId)
        {
            return InvokeAsync<object>("gameService", "selectChampionSkin", ChampionId, SkinId);
        }

        /// <summary>
        /// Lock in your champion selection
        /// </summary>
        public static Task<object> ChampionSelectCompleted()
        {
            return InvokeAsync<object>("gameService", "championSelectCompleted");
        }

        /// <summary>
        /// Gets the spectator game info for a summoner
        /// </summary>
        /// <param name="SummonerName">The summoner name</param>
        /// <returns>Returns the game info</returns>
        public static Task<PlatformGameLifecycleDTO> RetrieveInProgressSpectatorGameInfo(String SummonerName)
        {
            return InvokeAsync<PlatformGameLifecycleDTO>("gameService", "retrieveInProgressSpectatorGameInfo", SummonerName);
        }

        public static Task<PlatformGameLifecycleDTO> RetrieveInProgressGameInfo()
        {
            return InvokeAsync<PlatformGameLifecycleDTO>("gameService", "retrieveInProgressGameInfo");
        }

        /// <summary>
        /// Accepts a popped queue
        /// </summary>
        /// <param name="AcceptGame">Accept or decline the queue</param>
        public static Task<object> AcceptPoppedGame(Boolean AcceptGame)
        {
            return InvokeAsync<object>("gameService", "acceptPoppedGame", AcceptGame);
        }

        /// <summary>
        /// Bans a user from a custom game
        /// </summary>
        /// <param name="GameId">The game id</param>
        /// <param name="AccountId">The account id of the player</param>
        public static Task<object> BanUserFromGame(Double GameId, Double AccountId)
        {
            return InvokeAsync<object>("gameService", "banUserFromGame", GameId, AccountId);
        }

        /// <summary>
        /// Bans a user from a custom game
        /// </summary>
        /// <param name="GameId">The game id</param>
        /// <param name="AccountId">The account id of the player</param>
        public static Task<object> BanObserverFromGame(Double GameId, Double AccountId)
        {
            return InvokeAsync<object>("gameService", "banObserverFromGame", GameId, AccountId);
        }

        /// <summary>
        /// Bans a champion from the game (must be during PRE_CHAMP_SELECT and the users PickTurn)
        /// </summary>
        /// <param name="ChampionId">The champion id</param>
        public static Task<object> BanChampion(Int32 ChampionId)
        {
            return InvokeAsync<object>("gameService", "banChampion", ChampionId);
        }

        /// <summary>
        /// Gets the champions from the other team to ban
        /// </summary>
        /// <returns>Returns an array of champions to ban</returns>
        public static Task<ChampionBanInfoDTO[]> GetChampionsForBan()
        {
            return InvokeAsync<ChampionBanInfoDTO[]>("gameService", "getChampionsForBan");
        }

        /// <summary>
        /// Saves the mastery book
        /// </summary>
        /// <param name="MasteryBookPage">The mastery book information</param>
        /// <returns>Returns the mastery book</returns>
        public static Task<MasteryBookDTO> SaveMasteryBook(MasteryBookDTO MasteryBookPage)
        {
            return InvokeAsync<MasteryBookDTO>("masteryBookService", "saveMasteryBook", MasteryBookPage);
        }

        /// <summary>
        /// Gets the queue information for a selected queue
        /// </summary>
        /// <param name="QueueId">The queue id</param>
        /// <returns>Returns the queue information</returns>
        public static Task<QueueInfo> GetQueueInformation(Double QueueId)
        {
            return InvokeAsync<QueueInfo>("matchmakerService", "getQueueInfo", QueueId);
        }

        /// <summary>
        /// Gets the summoner icons for a user
        /// </summary>
        /// <param name="SummonerId">The summoner id</param>
        /// <returns>Returns the summoner icons</returns>
        public static Task<SummonerIconInventoryDTO> GetSummonerIconInventory(Double SummonerId)
        {
            return InvokeAsync<SummonerIconInventoryDTO>("summonerIconService", "getSummonerIconInventory", SummonerId);
        }

        /// <summary>
        /// Gets the allowed traders in the current game
        /// </summary>
        /// <returns>Returns a list of traders</returns>
        public static Task<PotentialTradersDTO> GetPotentialTraders()
        {
            return InvokeAsync<PotentialTradersDTO>("lcdsChampionTradeService", "getPotentialTraders");
        }

        /// <summary>
        /// Attempts to trade with a player
        /// </summary>
        /// <param name="SummonerInternalName">The internal name of a summoner</param>
        /// <param name="ChampionId">The champion id requested</param>
        public static Task<object> AttemptTrade(String SummonerInternalName, Int32 ChampionId)
        {
            return InvokeAsync<object>("lcdsChampionTradeService", "attemptTrade", SummonerInternalName, ChampionId, false);
        }

        /// <summary>
        /// Decline the current trade
        /// </summary>
        public static Task<object> DeclineTrade()
        {
            return InvokeAsync<object>("lcdsChampionTradeService", "dismissTrade");
        }

        /// <summary>
        /// Accepts the current trade
        /// </summary>
        /// <param name="SummonerInternalName">The internal name of a summoner</param>
        /// <param name="ChampionId">The champion id requested</param>
        public static Task<object> AcceptTrade(String SummonerInternalName, Int32 ChampionId)
        {
            return InvokeAsync<object>("lcdsChampionTradeService", "attemptTrade", SummonerInternalName, ChampionId, true);
        }

        public static Task<T> InvokeAsync<T>(string destination, string method, params object[] argument)
        {
            try
            {
                return Client.RtmpConnection.InvokeAsync<T>("my-rtmps", destination, method, argument);
            }
            catch (InvocationException e)
            {
                if (OnInvocationError != null)
                    OnInvocationError(null, e);
                return null;
            }
        }

        public class QueueTicker
        {
            public int id { get; set; }
            public int node { get; set; }
            public string champ { get; set; }
            public int current { get; set; }
        }

        public class QueueIngameCredentials
        {
            public bool inGane { get; set; }
            public object summonerId { get; set; }
            public object serverIp { get; set; }
            public object serverPort { get; set; }
            public object encryptionKey { get; set; }
            public object handshakeToken { get; set; }
        }

        public class QueueJson
        {
            public int node { get; set; }
            public int vcap { get; set; }
            public int rate { get; set; }

            public List<QueueTicker> tickers { get; set; }

            public int backlog { get; set; }
            public string reason { get; set; }
            public string status { get; set; }
            public string champ { get; set; }
            public int delay { get; set; }

            public QueueIngameCredentials inGameCredentials { get; set; }
            public string user { get; set; }
        }

        public class LoginJson
        {
            public int rate { get; set; }

            public string token { get; set; }

            public string reason { get; set; }

            public string status { get; set; }

            public int delay { get; set; }

            public QueueIngameCredentials inGameCredentials { get; set; }

            public string user { get; set; }
        }

        public static string GetAuthKey(String Username, String Password, String LoginQueue)
        {
            StringBuilder sb = new StringBuilder();
            string payload = "user=" + Username + ",password=" + Password;
            string query = "payload=" + payload;

            WebRequest authRequest = WebRequest.Create(LoginQueue + "login-queue/rest/queue/authenticate");
            authRequest.Method = "POST";

            Stream authOutput = authRequest.GetRequestStream();
            authOutput.Write(Encoding.ASCII.GetBytes(query), 0, Encoding.ASCII.GetByteCount(query));

            WebResponse authResponse = authRequest.GetResponse();
            Stream authInput = authResponse.GetResponseStream();

            int c;
            while ((c = authInput.ReadByte()) != -1)
                sb.Append((char)c);

            authOutput.Close();
            authInput.Close();
            authRequest.Abort();
            authResponse.Close();

            JavaScriptSerializer serializer = new JavaScriptSerializer();

            Dictionary<string, object> deserializedJSON = serializer.Deserialize<Dictionary<string, object>>(sb.ToString());

            //Do we have to wait in queue?
            if ((string)deserializedJSON["status"] == "LOGIN")
            {
                LoginJson deserializedLogin = serializer.Deserialize<LoginJson>(sb.ToString());
                return deserializedLogin.token;
            }
            else if ((string)deserializedJSON["status"] == "FAILED")
            {
                string reason = (string)deserializedJSON["reason"];
                Console.WriteLine("Failed to logging in: " + reason);
            }
            else if ((string)deserializedJSON["status"] == "QUEUE")
            {
                QueueJson deserializedLogin = serializer.Deserialize<QueueJson>(sb.ToString());

                //Read URL
                WebClient client = new WebClient();

                while (true)
                {
                    int id = 0;
                    int cur = 0;

                    //Get our node
                    foreach (QueueTicker ticker in deserializedLogin.tickers)
                    {
                        //Find our queue
                        if (ticker.node == deserializedLogin.node)
                        {
                            id = ticker.id;
                            cur = ticker.current;
                            break;
                        }

                        Console.WriteLine("In login queue #" + (id - cur).ToString());

                        //Request the queue until there's only 'rate' left to go
                        while (id - cur > deserializedLogin.rate)
                        {
                            //Sleep until the queue updates
                            System.Threading.Thread.Sleep(deserializedLogin.delay);

                            //Request queue infos
                            string queueInfo = null;
                            GetQueue:
                            try
                            {
                                queueInfo = client.DownloadString(LoginQueue + "login-queue/rest/queue/ticker/" + deserializedLogin.champ);
                            }
                            catch (Exception e)
                            {
                                System.Threading.Thread.Sleep(deserializedLogin.delay);
                                goto GetQueue;
                            }

                            //Get cur position
                            Dictionary<string, object> deserializedQueue = serializer.Deserialize<Dictionary<string, object>>(queueInfo);
                            cur = (int)deserializedQueue[deserializedLogin.node.ToString()];
                            Console.WriteLine("#" + (id - cur).ToString());
                        }
                    }

                    //Console.WriteLine("Getting token...");

                    TryAgain:;
                    try
                    {
                        string authResp = client.DownloadString(LoginQueue + "login-queue/rest/queue/authToken/" + Username.ToLower());
                        deserializedJSON = serializer.Deserialize<Dictionary<string, object>>(authResp);

                        string status = (string)deserializedJSON["status"];
                        //System.Threading.Thread.Sleep(1000);

                        if (status == "JOIN")
                        {
                            return (string)deserializedJSON["token"];
                            break;
                        }
                    }
                    catch (Exception e)
                    {
                        System.Threading.Thread.Sleep(500);
                        goto TryAgain;
                    }
                }
            }
            else
            {
                Console.WriteLine("...");
                return GetAuthKey(Username, Password, LoginQueue);
            }

            return null;
        }

        public static string GetIpAddress()
        {
            StringBuilder sb = new StringBuilder();

            WebRequest con = WebRequest.Create("http://ll.leagueoflegends.com/services/connection_info");
            WebResponse response = con.GetResponse();

            int c;
            while ((c = response.GetResponseStream().ReadByte()) != -1)
                sb.Append((char)c);

            con.Abort();

            JavaScriptSerializer serializer = new JavaScriptSerializer();
            Dictionary<string, string> deserializedJSON = serializer.Deserialize<Dictionary<string, string>>(sb.ToString());

            return deserializedJSON["ip_address"];
        }

        public static SerializationContext RegisterObjects()
        {
            var context = new SerializationContext();

            Assembly LegendaryClient = Assembly.GetExecutingAssembly();

            var x = LegendaryClient.GetTypes().Where(t => String.Equals(t.Namespace, "LegendaryClient.Logic.Riot.Platform", StringComparison.Ordinal));
            foreach (Type Platform in x)
                context.Register(Platform);

            x = LegendaryClient.GetTypes().Where(t => String.Equals(t.Namespace, "LegendaryClient.Logic.Riot.Leagues", StringComparison.Ordinal));
            foreach (Type League in x)
                context.Register(League);

            x = LegendaryClient.GetTypes().Where(t => String.Equals(t.Namespace, "LegendaryClient.Logic.Riot.Team", StringComparison.Ordinal));
            foreach (Type Team in x)
                context.Register(Team);

            context.Register(typeof(PendingKudosDTO));
            context.RegisterAlias(typeof(Icon), "com.riotgames.platform.summoner.icon.SummonerIcon", true);
            context.RegisterAlias(typeof(StoreAccountBalanceNotification), "com.riotgames.platform.messaging.StoreAccountBalanceNotification", true);

            //Hack to make aram work
            context.RegisterAlias(typeof(PlayerParticipant), "com.riotgames.platform.reroll.pojo.AramPlayerParticipant", true);

            return context;
        }
    }
}