using PvPNetClient.Riot.com.riotgames.platform.gameinvite.contract;
using RtmpSharp.IO;
using System;

namespace com.riotgames.platform.gameinvite.contract
{
    [Serializable]
    [SerializedName("com.riotgames.platform.gameinvite.contract.LobbyStatus")]
    public class LobbyStatus
    {
        [SerializedName("chatKey")]
        public String ChatKey { get; set; }

        [SerializedName("gameMetaData")]
        public String GameMetaData { get; set; }

        [SerializedName("owner")]
        public Player Owner;

        [SerializedName("members")]
        public Member[] Members { get; set; }

        [SerializedName("invitees")]
        public Invitee[] Invitees { get; set; }

        [SerializedName("invitationId")]
        public String InvitationId { get; set; }
    }
}