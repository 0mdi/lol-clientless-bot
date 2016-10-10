﻿using RtmpSharp.IO;
using System;

namespace LegendaryClient.Logic.Riot.Platform
{
    [Serializable]
    [SerializedName("com.riotgames.platform.trade.api.contract.TradeContractDTO")]
    public class TradeContractDTO
    {
        [SerializedName("requesterInternalSummonerName")]
        public String RequesterInternalSummonerName { get; set; }

        [SerializedName("requesterChampionId")]
        public Double RequesterChampionId { get; set; }

        [SerializedName("state")]
        public String State { get; set; }

        [SerializedName("responderChampionId")]
        public Double ResponderChampionId { get; set; }

        [SerializedName("responderInternalSummonerName")]
        public String ResponderInternalSummonerName { get; set; }
    }
}