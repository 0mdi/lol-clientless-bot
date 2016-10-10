using RtmpSharp.IO;
using System;

namespace LegendaryClient.Logic.Riot.Platform
{
    [Serializable]
    [SerializedName("com.riotgames.platform.statistics.team.TeamPlayerAggregatedStatsDTO")]
    public class TeamPlayerAggregatedStatsDTO
    {
        [SerializedName("playerId")]
        public Double PlayerId { get; set; }

        [SerializedName("aggregatedStats")]
        public AggregatedStats AggregatedStats { get; set; }
    }
}