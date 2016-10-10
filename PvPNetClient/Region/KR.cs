﻿using System;
using System.Net;

namespace LegendaryClient.Logic.Region
{
    public sealed class KR : BaseRegion
    {
        public override string RegionName
        {
            get { return "KR"; }
        }

        public override string InternalName
        {
            get { return "KR"; }
        }

        public override string ChatName
        {
            get { return "kr"; }
        }

        //No known news URI for Korea
        public override Uri NewsAddress
        {
            get { return new Uri(""); }
        }

        public override string Server
        {
            get { return "prod.kr.lol.riotgames.com"; }
        }

        public override string LoginQueue
        {
            get { return "https://lq.kr.lol.riotgames.com/"; }
        }

        public override string Locale
        {
            get { return "ko_KR"; }
        }

        public override IPAddress[] PingAddresses
        {
            get
            {
                return new IPAddress[]
                {
                    //No known IP address
                };
            }
        }

        public override Uri SpectatorLink
        {
            get { return new Uri("http://QFKR1PROXY.kassad.in:8088/observer-mode/rest/"); }
        }

        public override string SpectatorIpAddress
        {
            get { return "110.45.191.11:80"; }
        }
    }
}