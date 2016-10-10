using System;
using System.Net;

namespace UpdateUpdater
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            WebClient downloader = new WebClient();

            //Update Updater
            try
            {
                Console.WriteLine("Downloading Updater");
                downloader.DownloadFile("*Removed*", "Updater.exe");
                Console.WriteLine("Download finished!");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while downloading Updater - " + e.Message);
            }

            //Start Updater
            System.Diagnostics.Process.Start("Updater.exe");
        }
    }
}