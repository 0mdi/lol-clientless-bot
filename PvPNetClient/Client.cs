using LegendaryClient.Logic.Riot;
using LegendaryClient.Logic.Riot.Platform;
using PvPNetClient;
using RtmpSharp.Net;
using System;
using System.Management;
using System.Timers;
using Timer = System.Timers.Timer;

namespace LegendaryClient.Logic
{
    internal static class Client
    {
        internal static string botType;
        internal static double accId;
        internal static string summonerName;
        internal static System.Timers.Timer HeartbeatTimer;
        internal static int HeartbeatCount;
        internal static bool selectedChampion = false;
        internal static bool heartbeatSet = false;

        internal static void StartHeartbeat()
        {
            HeartbeatTimer = new Timer(60000 * 2);
            HeartbeatTimer.Elapsed += new ElapsedEventHandler(DoHeartbeat);
            HeartbeatTimer.AutoReset = true;
            HeartbeatTimer.Start();
        }

        internal async static void DoHeartbeat(object sender, ElapsedEventArgs e)
        {
            if (Program.botLevelLimit == "LEVEL_30" && Program.groupSplitted)
            {
                bool exist = false;

                for (int i = 0; i < 2; ++i)
                {
                    ManagementClass mgmtClass = new ManagementClass("Win32_Process");
                    foreach (ManagementObject process in mgmtClass.GetInstances())
                    {
                        if (process["Name"].ToString().ToLower() == "clientemu.exe")
                        {
                            var commandLine = process["CommandLine"].ToString();

                            string[] splitted = commandLine.Split(' ');

                            if (splitted.Length >= 7)
                            {
                                if (splitted[6] == summonerName)
                                {
                                    exist = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                if (exist == false)
                {
                    //Check if game is running
                    try
                    {
                        var gameInfo = await RiotCalls.RetrieveInProgressGameInfo();

                        if (gameInfo.PlayerCredentials != null)
                        {
                            Console.WriteLine("Active Game Found: " + gameInfo.GameName);
                            Program.StartClient(gameInfo.PlayerCredentials);
                        }
                    }
                    catch (Exception ingameException)
                    {
                    }
                }
            }

            if (Program.botLevelLimit != "BOT" && Program.groupSplitted)
            {
                if (Program.inInviteLobby)
                {
                    await Program.InvitePlayers();
                }
            }

            try
            {
                await RiotCalls.PerformLCDSHeartBeat(Convert.ToInt32(Client.accId), PlayerSession.Token, HeartbeatCount,
                          DateTime.Now.ToString("ddd MMM d yyyy HH:mm:ss 'GMT-0700'"));
                HeartbeatCount++;
            }
            catch (Exception exception)
            {
                Console.WriteLine("Heartbeat failed! : " + exception.Message);
                Program.restart();
            }
        }

        internal static RtmpClient RtmpConnection;
        internal static Session PlayerSession;
    }
}