using RtmpSharp.IO;
using System;
using System.Collections.Generic;

namespace LegendaryClient.Logic.Riot.Platform
{
    [Serializable]
    [SerializedName("com.riotgames.platform.game.StartChampSelectDTO")]
    public class StartChampSelectDTO
    {
        [SerializedName("invalidPlayers")]
        public List<object> InvalidPlayers { get; set; }
    }
}