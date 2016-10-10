using RtmpSharp.IO;
using System;

namespace LegendaryClient.Logic.Riot.Platform
{
    [Serializable]
    [SerializedName("com.riotgames.platform.summoner.RuneSlot")]
    public class RuneSlot
    {
        [SerializedName("id")]
        public Int32 Id { get; set; }

        [SerializedName("minLevel")]
        public Int32 MinLevel { get; set; }

        [SerializedName("runeType")]
        public RuneType RuneType { get; set; }
    }
}