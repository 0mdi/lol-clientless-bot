using RtmpSharp.IO;
using System;

namespace LegendaryClient.Logic.Riot.Kudos
{
    [Serializable]
    [SerializedName("com.riotgames.kudos.dto.PendingKudosDTO")]
    public class PendingKudosDTO
    {
        [SerializedName("pendingCounts")]
        public Int32[] PendingCounts { get; set; }
    }
}