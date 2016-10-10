using LegendaryClient.Logic.Riot.Leagues;
using RtmpSharp.IO;
using System;
using System.Collections.Generic;

namespace LegendaryClient.Logic.Riot.Platform
{
    [Serializable]
    [SerializedName("com.riotgames.platform.leagues.client.dto.SummonerLeaguesDTO")]
    public class SummonerLeaguesDTO
    {
        [SerializedName("summonerLeagues")]
        public List<LeagueListDTO> SummonerLeagues { get; set; }
    }
}