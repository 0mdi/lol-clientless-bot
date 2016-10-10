using RtmpSharp.IO;
using System;
using System.Collections.Generic;

namespace LegendaryClient.Logic.Riot.Platform
{
    [Serializable]
    [SerializedName("com.riotgames.platform.statistics.PlayerStatSummaries")]
    public class PlayerStatSummaries
    {
        [SerializedName("season")]
        public Int32 season { get; set; }

        [SerializedName("dataVersion")]
        public Int32 dataVersion { get; set; }

        [SerializedName("playerStatSummarySet")]
        public List<PlayerStatSummary> PlayerStatSummarySet { get; set; }

        [SerializedName("userId")]
        public Double UserId { get; set; }

        [SerializedName("futureData")]
        public object futureData { get; set; }
    }
}