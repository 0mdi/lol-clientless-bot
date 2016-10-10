﻿using RtmpSharp.IO;
using System;
using System.Collections.Generic;

namespace LegendaryClient.Logic.Riot.Team
{
    [Serializable]
    [SerializedName("com.riotgames.team.dto.RosterDTO")]
    public class RosterDTO
    {
        [SerializedName("ownerId")]
        public Double OwnerId { get; set; }

        [SerializedName("memberList")]
        public List<TeamMemberInfoDTO> MemberList { get; set; }
    }
}