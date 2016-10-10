using RtmpSharp.IO;
using System;
using System.Collections.Generic;

namespace LegendaryClient.Logic.Riot.Platform
{
    [Serializable]
    [SerializedName("com.riotgames.platform.trade.api.contract.PotentialTradersDTO")]
    public class PotentialTradersDTO
    {
        [SerializedName("potentialTraders")]
        public List<String> PotentialTraders { get; set; }
    }
}