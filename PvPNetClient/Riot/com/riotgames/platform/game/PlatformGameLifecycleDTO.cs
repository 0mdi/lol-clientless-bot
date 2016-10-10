﻿using RtmpSharp.IO;
using System;

namespace LegendaryClient.Logic.Riot.Platform
{
    [Serializable]
    [SerializedName("com.riotgames.platform.game.PlatformGameLifecycleDTO")]
    public class PlatformGameLifecycleDTO
    {
        [SerializedName("gameSpecificLoyaltyRewards")]
        public object GameSpecificLoyaltyRewards { get; set; }

        [SerializedName("reconnectDelay")]
        public Int32 ReconnectDelay { get; set; }

        [SerializedName("lastModifiedDate")]
        public object LastModifiedDate { get; set; }

        [SerializedName("game")]
        public GameDTO Game { get; set; }

        [SerializedName("playerCredentials")]
        public PlayerCredentialsDto PlayerCredentials { get; set; }

        [SerializedName("gameName")]
        public String GameName { get; set; }

        [SerializedName("connectivityStateEnum")]
        public object ConnectivityStateEnum { get; set; }
    }
}