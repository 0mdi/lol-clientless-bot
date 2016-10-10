using RtmpSharp.IO;
using System;

namespace PvPNetClient.Riot.com.riotgames.platform.gameinvite.contract
{
    [Serializable]
    [SerializedName("com.riotgames.platform.gameinvite.contract.Invitee")]
    public class Invitee
    {
        [SerializedName("inviteeStateAsString")]
        private String InviteStateAsString { get; set; }

        [SerializedName("summonerName")]
        private String SummonerName { get; set; }

        [SerializedName("inviteeState")]
        private String InviteeState { get; set; }

        [SerializedName("summonerId")]
        private double SummonerId { get; set; }
    }
}