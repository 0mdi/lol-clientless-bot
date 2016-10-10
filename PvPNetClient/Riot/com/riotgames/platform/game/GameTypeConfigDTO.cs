using RtmpSharp.IO;
using System;

namespace LegendaryClient.Logic.Riot.Platform
{
    [Serializable]
    [SerializedName("com.riotgames.platform.game.GameTypeConfigDTO")]
    public class GameTypeConfigDTO
    {
        [SerializedName("allowTrades")]
        public Boolean AllowTrades { get; set; }

        [SerializedName("banTimerDuration")]
        public Int32 BanTimerDuration { get; set; }

        [SerializedName("maxAllowableBans")]
        public Int32 MaxAllowableBans { get; set; }

        [SerializedName("crossTeamChampionPool")]
        public Boolean CorssTeamChampionPool { get; set; }

        [SerializedName("teamChampionPool")]
        public Boolean TeamChampionPool { get; set; }

        [SerializedName("postPickTimerDuration")]
        public Int32 PostPickTimerDuration { get; set; }

        [SerializedName("id")]
        public Int32 Id { get; set; }

        [SerializedName("duplicatePick")]
        public Boolean DuplicatePick { get; set; }

        [SerializedName("dataVersion")]
        public Int32 DataVersion { get; set; }

        [SerializedName("exclusivePick")]
        public Boolean ExclusivePick { get; set; }

        [SerializedName("mainPickTimerDuration")]
        public Int32 MainPickTimerDuration { get; set; }

        [SerializedName("name")]
        public String Name { get; set; }

        [SerializedName("pickMode")]
        public String PickMode { get; set; }
    }
}