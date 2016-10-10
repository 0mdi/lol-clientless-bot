using RtmpSharp.IO;
using System;

namespace LegendaryClient.Logic.Riot.Platform
{
    [Serializable]
    [SerializedName("com.riotgames.platform.harassment.LcdsResponseString")]
    public class LcdsResponseString
    {
        [SerializedName("value")]
        public String Value { get; set; }
    }
}