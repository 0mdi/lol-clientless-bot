using DeathByCaptcha;
using System;
using System.Collections.Generic;
using System.IO;
using System.Net.Http;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Exception = System.Exception;
using HttpClient = System.Net.Http.HttpClient;

namespace AccountCreator
{
    internal class Program
    {
        private static string refLink;
        private static int refCount;
        private static RandomNameGenerator.PersonNameGenerator nameGenerator = new RandomNameGenerator.PersonNameGenerator();
        private static Random randomGenerator = new Random();
        private static System.Threading.Mutex mutex = new System.Threading.Mutex();
        private static System.Threading.Mutex nameMutext = new System.Threading.Mutex();
        internal static StreamWriter logFile;
        internal static string platform;

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        public static extern void OutputDebugString(string message);

        public static string Generate()
        {
            nameMutext.WaitOne();

            string randomName = nameGenerator.GenerateRandomFirstName() + nameGenerator.GenerateRandomLastName();

            while (randomName.Length >= 13)
                randomName = nameGenerator.GenerateRandomFirstName() + nameGenerator.GenerateRandomLastName();

            nameMutext.ReleaseMutex();

            return randomName;
        }

        public class SolvedCaptcha
        {
            public Captcha captcha;
            public string challenge;
        }

        private static async Task<SolvedCaptcha> CrackCaptcha(DeathByCaptcha.Client dbc_client, HttpClient client, string content)
        {
            var recaptcha_id = content.Split(new string[] { "Recaptcha.create(\"" }, StringSplitOptions.None)[1]
                .Split('\"')[0]
                .Trim();

            var challenge_response =
                await client.GetStringAsync("https://www.google.com/recaptcha/api/challenge?k=" + recaptcha_id);

            Regex regexChallenge = new Regex("challenge : '(.*)'");
            var match = regexChallenge.Match(challenge_response);

            if (!match.Success)
                return null;

            var recaptcha_challenge = match.Groups[1].Value;

            var image_response =
                await client.GetByteArrayAsync("https://www.google.com/recaptcha/api/image?c=" + recaptcha_challenge);

            if (image_response == null)
                return null;

            Captcha captcha = dbc_client.Decode(image_response);

            if (captcha.Solved && captcha.Correct)
            {
                SolvedCaptcha solved = new SolvedCaptcha();
                solved.captcha = captcha;
                solved.challenge = recaptcha_challenge;

                return solved;
            }
            else
                return null;
        }

        private static string GetTokenKey(string content)
        {
            var stringArray = content.Split('"');

            for (int i = 0; i < stringArray.Length; ++i)
            {
                if (stringArray[i].Contains("data[_Token][key]"))
                {
                    return stringArray[i + 2];
                }
            }
            return "ERROR!";
        }

        private static string GetTokenFields(string content)
        {
            var stringArray = content.Split('"');

            for (int i = 0; i < stringArray.Length; ++i)
            {
                if (stringArray[i].Contains("data[_Token][fields]"))
                {
                    return stringArray[i + 2];
                }
            }

            return "ERROR!";
        }

        private static async Task<int> CreateAccounts(int refferalCount)
        {
            string[] emailDomains =
            {
                "@hotmail.com", "@ok.de", "@yahoo.com", "@yahoo.de",
                "@ymail.com", "@rocketmail.com", "@mail.ru", "@googlemail.com",
                "@gmail.de", "@outlook.com", "@hotmail.de", "@live.de", "@hotmail.com",
                "@t-online.de"
            };

            OutputDebugString("Creating accounts[" + refferalCount.ToString() + "]");
            DeathByCaptcha.Client dbc_client = (DeathByCaptcha.Client)new SocketClient("Username", "Password"); ;
            OutputDebugString("DBC Success!");

            try
            {
                for (int i = 0; i < refferalCount; ++i)
                {
                    retry:
                    try
                    {
                        using (HttpClient client = new HttpClient())
                        using (HttpContent content = client.GetAsync(refLink).Result.Content)
                        {
                            bool created = false;

                            while (!created)
                            {
                                string strContent = content.ReadAsStringAsync().Result;

                                string username = Generate() + randomGenerator.Next(99).ToString();
                                string password = Generate() + randomGenerator.Next(10).ToString();
                                var captcha = CrackCaptcha(dbc_client, client, strContent).Result;
                                string tokenKey = GetTokenKey(strContent);
                                string tokenFields = GetTokenFields(strContent);

                                var pairs = new List<KeyValuePair<string, string>>
                            {
                                new KeyValuePair<string, string>("_method", "POST"),
                                new KeyValuePair<string, string>("data[_Token][key]", tokenKey),
                                new KeyValuePair<string, string>("data[PvpnetAccount][name]", username),
                                new KeyValuePair<string, string>("data[PvpnetAccount][password]", password),
                                new KeyValuePair<string, string>("data[PvpnetAccount][confirm_password]", password),
                                new KeyValuePair<string, string>("data[PvpnetAccount][email_address]", Generate() + emailDomains[randomGenerator.Next(0, emailDomains.Length)]),
                                new KeyValuePair<string, string>("data[PvpnetAccount][date_of_birth_day]", randomGenerator.Next(1, 29).ToString()),
                                new KeyValuePair<string, string>("data[PvpnetAccount][date_of_birth_month]", randomGenerator.Next(1, 13).ToString()),
                                new KeyValuePair<string, string>("data[PvpnetAccount][date_of_birth_year]", randomGenerator.Next(1990, 1999).ToString()),
                                new KeyValuePair<string, string>("data[PvpnetAccount][realm]", platform),
                                new KeyValuePair<string, string>("data[PvpnetAccount][tou_agree]", "0"),
                                new KeyValuePair<string, string>("data[PvpnetAccount][tou_agree]", "1"),
                                new KeyValuePair<string, string>("data[PvpnetAccount][newsletter]", randomGenerator.Next(2).ToString()),
                                new KeyValuePair<string, string>("recaptcha_response_field", captcha.captcha.Text),
                                new KeyValuePair<string, string>("recaptcha_challenge_field", captcha.challenge),
                                new KeyValuePair<string, string>("data[_Token][fields]", tokenFields),
                            };

                                if (captcha != null)
                                {
                                    OutputDebugString("CAPTCHA:" + captcha.captcha.Text);
                                    var postData = new FormUrlEncodedContent(pairs);

                                    repost:
                                    try
                                    {
                                        var response2 = client.PostAsync(new Uri("https://signup.leagueoflegends.com/de/signup/index"), postData).Result;
                                        var responseContent = response2.Content.ReadAsStringAsync().Result;
                                        //System.IO.File.WriteAllText("Reponse2.html", responseContent);

                                        if (!responseContent.Contains("Dein Konto wurde erstellt!"))
                                        {
                                            OutputDebugString("ACC: FAILED! -> " + username + ":" + password + ":" + captcha.captcha.Text);

                                            if (responseContent.Contains("The reCaptcha code was incorrect"))
                                                dbc_client.Report(captcha.captcha);
                                        }
                                        else
                                        {
                                            OutputDebugString(username + ":" + password);
                                            Console.WriteLine(username + ":" + password);
                                            created = true;
                                        }
                                    }
                                    catch (Exception e)
                                    {
                                        OutputDebugString("EXCEPTION____ : " + e.Message);
                                    }
                                }
                            }
                        }
                    }
                    catch (Exception e)
                    {
                        OutputDebugString("EXCEPTION : " + e.Message);
                    }
                }
            }
            catch (Exception e)
            {
                OutputDebugString("CreateAccounts() => " + e.Message);
            }

            return 1;
        }

        private static void Main(string[] args)
        {
            refLink = args[0];
            refCount = Convert.ToInt32(args[1]);
            string overwrite = args[2];
            platform = args[3];

            Console.WriteLine("refLink: " + refLink);
            Console.WriteLine("refCount: " + args[1]);
            Console.WriteLine("overwrite: " + overwrite);
            Console.WriteLine("platform: " + platform);

            try
            {
                if (overwrite == "yes")
                {
                    File.Delete("Accounts.txt");
                    logFile = new StreamWriter(new FileStream("Accounts.txt", FileMode.Create));
                }
                else if (overwrite == "no")
                {
                    logFile = new StreamWriter(new FileStream("Accounts.txt", FileMode.Append));
                }
            }
            catch (Exception e)
            {
                OutputDebugString("Error with I/O Accounts.txt : " + e.Message);
            }

            logFile.AutoFlush = true;
            Console.SetOut(logFile);

            int taskCount = refCount / 5;
            int rest = refCount % 5;

            if (taskCount != 0)
            {
                Task[] tasks = new Task[taskCount];

                for (int i = 0; i < taskCount; ++i)
                {
                    tasks[i] = Task.Run(async () => await CreateAccounts(5));
                }

                try
                {
                    Task.WaitAll(tasks);
                }
                catch (AggregateException e)
                {
                    OutputDebugString("Task.WaitAll Exception: " + e.Message);
                    for (int j = 0; j < e.InnerExceptions.Count; ++j)
                    {
                        OutputDebugString(("\n-------------------------------------------------\n" + e.InnerExceptions[j].ToString()));
                    }
                }
            }
            else
            {
                if (rest != 0)
                {
                    Task task = Task.Run(async () => await CreateAccounts(rest));

                    try
                    {
                        task.Wait();
                    }
                    catch (AggregateException e)
                    {
                        OutputDebugString("task.Wait() Exception: " + e.Message);
                        for (int j = 0; j < e.InnerExceptions.Count; ++j)
                        {
                            OutputDebugString(("\n-------------------------------------------------\n" + e.InnerExceptions[j].ToString()));
                        }
                    }
                }
            }
        }
    }
}