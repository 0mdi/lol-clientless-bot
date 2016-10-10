using RtmpSharp.IO;
using System;

namespace PvPNetClient.Riot.com.riotgames.platform.gameinvite.contract
{
    [Serializable]
    [SerializedName("com.riotgames.platform.gameinvite.contract.InvitationRequest")]
    public class InvitationRequest
    {
        [SerializedName("inviter")]
        public object Inviter { get; set; }

        [SerializedName("inviteType")]
        public String InviteType { get; set; }

        [SerializedName("gameMetaData")]
        public String GameMetaData { get; set; }

        [SerializedName("owner")]
        public Player Owner { get; set; }

        [SerializedName("invitationStateAsString")]
        public String InvitationStateAsString { get; set; }

        [SerializedName("invitationState")]
        public String InvitationState { get; set; }

        [SerializedName("inviteTypeAsString")]
        public String InviteTypeAsString { get; set; }

        [SerializedName("invitationId")]
        public String InvitationId { get; set; }
    }
}