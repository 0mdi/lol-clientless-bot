﻿using System;
using System.Net;

namespace LegendaryClient.Logic.Region
{
    public abstract class BaseRegion
    {
        public abstract string RegionName { get; }

        public abstract string InternalName { get; }

        public abstract string ChatName { get; }

        public abstract Uri NewsAddress { get; }

        public abstract string Server { get; }

        public abstract string LoginQueue { get; }

        public abstract string Locale { get; }

        public abstract IPAddress[] PingAddresses { get; }

        public abstract Uri SpectatorLink { get; }

        public abstract string SpectatorIpAddress { get; }

        public static BaseRegion GetRegion(String RequestedRegion)
        {
            RequestedRegion = RequestedRegion.ToUpper();
            Type t = Type.GetType("LegendaryClient.Logic.Region." + RequestedRegion);

            if (t != null)
            {
                return (BaseRegion)Activator.CreateInstance(t);
            }
            return null;
        }
    }
}