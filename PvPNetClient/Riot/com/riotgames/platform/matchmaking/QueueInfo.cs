using RtmpSharp.IO;
using System;

namespace LegendaryClient.Logic.Riot.Platform
{
    [Serializable]
    [SerializedName("com.riotgames.platform.matchmaking.QueueInfo")]
    public class QueueInfo
    {
        [SerializedName("waitTime")]
        public Double WaitTime { get; set; }

        [SerializedName("queueId")]
        public Double QueueId { get; set; }

        [SerializedName("queueLength")]
        public Int32 QueueLength { get; set; }
    }
}