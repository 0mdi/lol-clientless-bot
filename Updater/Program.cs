using System;
using System.Net;

namespace Updater
{
    internal class Program
    {
        /// <summary>
        /// Gens the CRC16.
        /// CRC-1021 = X(16)+x(12)+x(5)+1
        /// </summary>
        /// <param name="c">The c.</param>
        /// <param name="nByte">The n byte.</param>
        /// <returns>System.Byte[][].</returns>
        public static ushort GenCrc16(byte[] c, int nByte)
        {
            ushort Polynominal = 0x1021;
            ushort InitValue = 0x0;

            ushort i, j, index = 0;
            ushort CRC = InitValue;
            ushort Remainder, tmp, short_c;
            for (i = 0; i < nByte; i++)
            {
                short_c = (ushort)(0x00ff & (ushort)c[index]);
                tmp = (ushort)((CRC >> 8) ^ short_c);
                Remainder = (ushort)(tmp << 8);
                for (j = 0; j < 8; j++)
                {
                    if ((Remainder & 0x8000) != 0)
                    {
                        Remainder = (ushort)((Remainder << 1) ^ Polynominal);
                    }
                    else
                    {
                        Remainder = (ushort)(Remainder << 1);
                    }
                }
                CRC = (ushort)((CRC << 8) ^ Remainder);
                index++;
            }
            return CRC;
        }

        private static void Main(string[] args)
        {
            WebClient downloader = new WebClient();

            //Update ControlPanel
            try
            {
                Console.WriteLine("Downloading ControlPanel");
                downloader.DownloadFile("*Removed*", "ControlPanel.exe");
                Console.WriteLine("Download finished!");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while downloading ControlPanel - " + e.Message);
            }

            //Update MasterControlPanel
            try
            {
                Console.WriteLine("Downloading MasterCPClient");
                downloader.DownloadFile("*Removed*", "MasterCPClient.exe");
                Console.WriteLine("Download finished!");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while downloading MasterCPClient - " + e.Message);
            }

            //Update Qt5Network.dll
            try
            {
                Console.WriteLine("Downloading Qt5Network.dll");
                downloader.DownloadFile("*Removed*", "Qt5Network.dll");
                Console.WriteLine("Download finished!");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while downloading Qt5Network.dll - " + e.Message);
            }

            //Update OrderReader
            try
            {
                Console.WriteLine("Downloading OrderReader");
                downloader.DownloadFile("*Removed*", "OrderReader.exe");
                Console.WriteLine("Download finished!");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while downloading OrderReader - " + e.Message);
            }

            //Updating AccountCreator
            try
            {
                Console.WriteLine("Downloading AccountCreator");
                downloader.DownloadFile("*Removed*", "AccountCreator.exe");
                Console.WriteLine("Download finished!");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while downloading AccountCreator - " + e.Message);
            }

            //Updating ClientEmu
            try
            {
                Console.WriteLine("Downloading ClientEmu");
                downloader.DownloadFile("*Removed*", "ClientEmu.exe");
                Console.WriteLine("Download finished!");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while downloading ClientEmu - " + e.Message);
            }

            //Updating PvPNetClient
            try
            {
                Console.WriteLine("Downloading PvPNetClient");
                downloader.DownloadFile("*Removed*", "PvPNetClient.exe");
                Console.WriteLine("Download finished!");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while downloading PvPNetClient - " + e.Message);
            }

            //Updating Dominion Script
            try
            {
                Console.WriteLine("Downloading Dominion.lua");
                downloader.DownloadFile("*Removed*", "Dominion.lua");
                Console.WriteLine("Download finished!");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while downloading Dominion.lua - " + e.Message);
            }

            //Updating COOPvsAI Script
            try
            {
                Console.WriteLine("Downloading COOPvsAI.lua");
                downloader.DownloadFile("*Removed*", "COOPvsAI.lua");
                Console.WriteLine("Download finished!");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while downloading COOPvsAI.lua - " + e.Message);
            }

            //Update DeathByCaptcha.dll
            try
            {
                Console.WriteLine("Downloading DeathByCaptcha.dll");
                downloader.DownloadFile("*Removed*", "DeathByCaptcha.dll");
                Console.WriteLine("Download finished!");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while downloading DeathByCaptcha.dll - " + e.Message);
            }
        }
    }
}