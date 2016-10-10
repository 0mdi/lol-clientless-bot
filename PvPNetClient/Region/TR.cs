using System;
using System.Net;

namespace LegendaryClient.Logic.Region
{
    public sealed class TR : BaseRegion
    {
        public override string RegionName
        {
            get { return "TR"; }
        }

        public override string InternalName
        {
            get { return "TR1"; }
        }

        public override string ChatName
        {
            get { return "tr"; }
        }

        public override Uri NewsAddress
        {
            get { return new Uri("http://ll.leagueoflegends.com/landingpage/data/tr/en_US.js"); } //This returns english (not spanish) characters
        }

        public override string Server
        {
            get { return "prod.tr.lol.riotgames.com"; }
        }

        public override string LoginQueue
        {
            get { return "https://lq.tr.lol.riotgames.com/"; }
        }

        public override string Locale
        {
            get { return "en_US"; }
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
            get { return new Uri("http://spectator.tr.lol.riotgames.com:80/observer-mode/rest/"); }
        }

        public override string SpectatorIpAddress
        {
            get { return "95.172.65.242:80"; }
        }
    }
}