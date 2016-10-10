using RtmpSharp.IO;
using System;

namespace PvPNetClient.Riot.com.riotgames.platform.gameinvite.contract
{
    [Serializable]
    [SerializedName("com.riotgames.platform.gameinvite.contract.Member")]
    public class Member
    {
        [SerializedName("hasDelegatedInvitePower")]
        public bool HasDelegatedInvitePower { get; set; }

        [SerializedName("summonerName")]
        public String SummonerName { get; set; }

        [SerializedName("summonerId")]
        public double SummonerId { get; set; }
    }
}