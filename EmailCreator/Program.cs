using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Net;
using System.Net.Http;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EmailCreator
{
    internal class Program
    {
        private static RandomNameGenerator.PersonNameGenerator nameGenerator = new RandomNameGenerator.PersonNameGenerator();
        private static Random randomGenerator = new Random();
        private static System.Threading.Mutex mutex = new System.Threading.Mutex();
        private static System.IO.FileStream outputFile = null;

        private static void AddText(FileStream fs, string value)
        {
            byte[] info = new UTF8Encoding(true).GetBytes(value);
            fs.Write(info, 0, info.Length);
        }

        public static DialogResult InputBox(string title, string promptText, ref string value, Bitmap image)
        {
            Form form = new Form();
            Label label = new Label();
            TextBox textBox = new TextBox();
            Button buttonOk = new Button();
            Button buttonCancel = new Button();
            PictureBox picture = new PictureBox();

            form.Text = title;
            label.Text = promptText;
            textBox.Text = value;
            picture.Image = image;

            buttonOk.Text = "OK";
            buttonCancel.Text = "Cancel";
            buttonOk.DialogResult = DialogResult.OK;
            buttonCancel.DialogResult = DialogResult.Cancel;

            label.SetBounds(9, 20, 372, 13);
            textBox.SetBounds(12, 36, 372, 20);
            buttonOk.SetBounds(228, 72, 75, 23);
            buttonCancel.SetBounds(309, 72, 75, 23);
            picture.SetBounds(14, 60, image.Width, image.Height);

            label.AutoSize = true;
            textBox.Anchor = textBox.Anchor | AnchorStyles.Right;
            buttonOk.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            buttonCancel.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            picture.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;

            form.ClientSize = new Size(396, 400); //Changed size to see the image
            form.ClientSize = new Size(Math.Max(300, label.Right + 10), form.ClientSize.Height); //Changed position so you are not shrinking the available size after the controls are added
            form.Controls.AddRange(new Control[] { label, textBox, buttonOk, buttonCancel, picture });
            form.FormBorderStyle = FormBorderStyle.FixedDialog;
            form.StartPosition = FormStartPosition.CenterScreen;
            form.MinimizeBox = false;
            form.MaximizeBox = false;
            form.AcceptButton = buttonOk;
            form.CancelButton = buttonCancel;

            DialogResult dialogResult = form.ShowDialog();
            value = textBox.Text;
            return dialogResult;
        }

        public class HSLColor
        {
            // Private data members below are on scale 0-1
            // They are scaled for use externally based on scale
            private double hue = 1.0;

            private double saturation = 1.0;
            private double luminosity = 1.0;

            private const double scale = 240.0;

            public double Hue
            {
                get { return hue * scale; }
                set { hue = CheckRange(value / scale); }
            }

            public double Saturation
            {
                get { return saturation * scale; }
                set { saturation = CheckRange(value / scale); }
            }

            public double Luminosity
            {
                get { return luminosity * scale; }
                set { luminosity = CheckRange(value / scale); }
            }

            private double CheckRange(double value)
            {
                if (value < 0.0)
                    value = 0.0;
                else if (value > 1.0)
                    value = 1.0;
                return value;
            }

            public override string ToString()
            {
                return String.Format("H: {0:#0.##} S: {1:#0.##} L: {2:#0.##}", Hue, Saturation, Luminosity);
            }

            public string ToRGBString()
            {
                Color color = (Color)this;
                return String.Format("R: {0:#0.##} G: {1:#0.##} B: {2:#0.##}", color.R, color.G, color.B);
            }

            #region Casts to/from System.Drawing.Color

            public static implicit operator Color(HSLColor hslColor)
            {
                double r = 0, g = 0, b = 0;
                if (hslColor.luminosity != 0)
                {
                    if (hslColor.saturation == 0)
                        r = g = b = hslColor.luminosity;
                    else
                    {
                        double temp2 = GetTemp2(hslColor);
                        double temp1 = 2.0 * hslColor.luminosity - temp2;

                        r = GetColorComponent(temp1, temp2, hslColor.hue + 1.0 / 3.0);
                        g = GetColorComponent(temp1, temp2, hslColor.hue);
                        b = GetColorComponent(temp1, temp2, hslColor.hue - 1.0 / 3.0);
                    }
                }
                return Color.FromArgb((int)(255 * r), (int)(255 * g), (int)(255 * b));
            }

            private static double GetColorComponent(double temp1, double temp2, double temp3)
            {
                temp3 = MoveIntoRange(temp3);
                if (temp3 < 1.0 / 6.0)
                    return temp1 + (temp2 - temp1) * 6.0 * temp3;
                else if (temp3 < 0.5)
                    return temp2;
                else if (temp3 < 2.0 / 3.0)
                    return temp1 + ((temp2 - temp1) * ((2.0 / 3.0) - temp3) * 6.0);
                else
                    return temp1;
            }

            private static double MoveIntoRange(double temp3)
            {
                if (temp3 < 0.0)
                    temp3 += 1.0;
                else if (temp3 > 1.0)
                    temp3 -= 1.0;
                return temp3;
            }

            private static double GetTemp2(HSLColor hslColor)
            {
                double temp2;
                if (hslColor.luminosity < 0.5)  //<=??
                    temp2 = hslColor.luminosity * (1.0 + hslColor.saturation);
                else
                    temp2 = hslColor.luminosity + hslColor.saturation - (hslColor.luminosity * hslColor.saturation);
                return temp2;
            }

            public static implicit operator HSLColor(Color color)
            {
                HSLColor hslColor = new HSLColor();
                hslColor.hue = color.GetHue() / 360.0; // we store hue as 0-1 as opposed to 0-360
                hslColor.luminosity = color.GetBrightness();
                hslColor.saturation = color.GetSaturation();
                return hslColor;
            }

            #endregion Casts to/from System.Drawing.Color

            public void SetRGB(int red, int green, int blue)
            {
                HSLColor hslColor = (HSLColor)Color.FromArgb(red, green, blue);
                this.hue = hslColor.hue;
                this.saturation = hslColor.saturation;
                this.luminosity = hslColor.luminosity;
            }

            public HSLColor()
            {
            }

            public HSLColor(Color color)
            {
                SetRGB(color.R, color.G, color.B);
            }

            public HSLColor(int red, int green, int blue)
            {
                SetRGB(red, green, blue);
            }

            public HSLColor(double hue, double saturation, double luminosity)
            {
                this.Hue = hue;
                this.Saturation = saturation;
                this.Luminosity = luminosity;
            }
        }

        public static Color Darken(Color color, double darkenAmount)
        {
            HSLColor hslColor = new HSLColor(color);
            hslColor.Luminosity *= darkenAmount; // 0 to 1
            return hslColor;
        }

        public static string GenerateFullName()
        {
            string randomName = nameGenerator.GenerateRandomFirstName() + nameGenerator.GenerateRandomLastName();

            while (randomName.Length >= 13)
                randomName = nameGenerator.GenerateRandomFirstName() + nameGenerator.GenerateRandomLastName();

            return randomName;
        }

        public static string GenerateFirstName()
        {
            return nameGenerator.GenerateRandomFirstName();
        }

        public static string GenerateLastName()
        {
            return nameGenerator.GenerateRandomLastName();
        }

        static public void FloodFill(Bitmap bitmap, int x, int y, System.Drawing.Color color)
        {
            BitmapData data = bitmap.LockBits(
              new Rectangle(0, 0, bitmap.Width, bitmap.Height),
              ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            int[] bits = new int[data.Stride / 4 * data.Height];
            Marshal.Copy(data.Scan0, bits, 0, bits.Length);

            LinkedList<Point> check = new LinkedList<Point>();
            int floodTo = color.ToArgb();
            int floodFrom = bits[x + y * data.Stride / 4];
            bits[x + y * data.Stride / 4] = floodTo;

            if (floodFrom != floodTo)
            {
                check.AddLast(new Point(x, y));
                while (check.Count > 0)
                {
                    Point cur = check.First.Value;
                    check.RemoveFirst();

                    foreach (Point off in new Point[] {
        new Point(0, -1), new Point(0, 1),
        new Point(-1, 0), new Point(1, 0)})
                    {
                        Point next = new Point(cur.X + off.X, cur.Y + off.Y);
                        if (next.X >= 0 && next.Y >= 0 &&
                          next.X < data.Width &&
                          next.Y < data.Height)
                        {
                            if (bits[next.X + next.Y * data.Stride / 4] == floodFrom)
                            {
                                check.AddLast(next);
                                bits[next.X + next.Y * data.Stride / 4] = floodTo;
                            }
                        }
                    }
                }
            }

            Marshal.Copy(bits, 0, data.Scan0, bits.Length);
            bitmap.UnlockBits(data);
        }

        static public void PrepareCaptcha(Bitmap captcha)
        {
            for (int i = 0; i < captcha.Width; i++)
            {
                for (int k = 0; k < captcha.Height; k++)
                {
                    captcha.SetPixel(i, k, Darken(captcha.GetPixel(i, k), 0.05));
                }
            }
        }

        static public async Task<Bitmap> LoadPicture(HttpClient client, string url)
        {
            Bitmap bmp;

            try
            {
                var stream = await client.GetStreamAsync(url);
                bmp = new Bitmap(stream);
            }
            finally
            {
            }
            return (bmp);
        }

        private static string CrackCaptcha(HttpClient client, string content)
        {
            try
            {
                //Console.WriteLine("CRACKCAPTCHA");
                Bitmap captchaBitmap = LoadPicture(client, "https://www.eclipso.de/index.php?action=codegen&id=" + content).Result;
                //PrepareCaptcha(captchaBitmap);
                //captchaBitmap.Save("captcha.png");

                string result = "~";

                InputBox("CAPTCHA", "Please enter the letters!", ref result, captchaBitmap);
                return result;

                //mutex.WaitOne();

                //using (tessnet2.Tesseract ocr = new tessnet2.Tesseract())
                //{
                //    ocr.Init(Directory.GetCurrentDirectory() + @"\tessdata\", "eng", false);

                //    List<tessnet2.Word> wlist = ocr.DoOCR(captchaBitmap, Rectangle.Empty);
                //    foreach (tessnet2.Word w in wlist)
                //        result += w.Text;

                //    ocr.Clear();
                //    ocr.Dispose();
                //    wlist.Clear();

                //    mutex.ReleaseMutex();

                //    return result;
                //}
            }
            catch (Exception e)
            {
                //Console.WriteLine(e.Message);
            }

            return "ERROR";
        }

        static public string[] GetProxyList()
        {
            var requestUrl = "http://letushide.com/fpapi/?key=d144888e2f5c70a2b1cb6af6&format=json&num=100&as=hap&ps=http";

            //Create a Web-Request to an URL
            HttpWebRequest httpWebRequest = (HttpWebRequest)WebRequest.Create(requestUrl);

            //Defined poperties for the Web-Request
            httpWebRequest.Method = "POST";
            httpWebRequest.MediaType = "HTTP/1.1";
            httpWebRequest.ContentType = "text/xml";
            httpWebRequest.UserAgent = "Example Client";

            //Send Web-Request and receive a Web-Response
            HttpWebResponse httpWebesponse = (HttpWebResponse)httpWebRequest.GetResponse();

            //Translate data from the Web-Response to a string
            Stream dataStream = httpWebesponse.GetResponseStream();
            StreamReader streamreader = new StreamReader(dataStream, Encoding.UTF8);
            string response = streamreader.ReadToEnd();
            streamreader.Close();

            dynamic json = System.Web.Helpers.Json.Decode(response);
            string[] proxyList = new string[json.count];

            for (int i = 0; i < json.count; ++i)
            {
                proxyList[i] = json.data[i].host + ":" + json.data[i].port;
            }

            return proxyList;
        }

        static public string GetCodeId(string content)
        {
            var stringArray = content.Split('"');

            for (int i = 0; i < stringArray.Length; ++i)
            {
                if (stringArray[i].Contains("codeID"))
                {
                    return stringArray[i + 2];
                }
            }

            return "ERROR";
        }

        private static async void CreateAccounts(int count, string[] proxyList)
        {
            string[] CityList =
            {
                "Köln", "Hamburg", "Berlin", "Kassel", "Düsseldorf",
                "Memmingen", "Northeim", "Göttingen", "Duderstadt",
                "Diemelstadt", "Bad Aibling", "Bad Belzig", "Bad Bibra",
                "Balve", "Barmstedt", "Barth", "Borkum", "Borken", "Bottrop",
                "Dachau", "Dahn", "Daun", "Datteln", "Döbeln", "Leipzig",
                "Dortmund", "Duisburg", "Mainz", "Ellingen"
            };

            try
            {
                int proxyCount = 0;

                for (int i = 0; i < count; ++i)
                {
                    bool created = false;

                    if (proxyCount > proxyList.Length)
                        proxyCount = 0;

                    //Console.WriteLine("Using: " + proxyList[proxyCount]);

                    try
                    {
                        using (HttpClient client = new HttpClient(
                                new HttpClientHandler
                                {
                                    Proxy = new WebProxy(new Uri("http://" + proxyList[proxyCount])),
                                    UseProxy = true,
                                }))
                        using (HttpResponseMessage responseMsg = client.GetAsync("https://www.eclipso.de/email-konto-anlegen.php").Result)
                        using (HttpContent content = responseMsg.Content)
                        {
                            string strContent = content.ReadAsStringAsync().Result;
                            var codeID = GetCodeId(strContent);

                            if (strContent.Contains("da Sie erst vor kurzem eine Registrierung durchgeführt haben"))
                            {
                                Console.WriteLine("IP BANNED");
                                ++proxyCount;
                                --i;
                                continue;
                            }

                            var firstName = GenerateFirstName();
                            var lastName = GenerateLastName();
                            var zipCode = randomGenerator.Next(29999, 59999);
                            var cityName = CityList[randomGenerator.Next(0, CityList.Length)];
                            var roadName = GenerateLastName() + "straße";
                            var roadNumber = randomGenerator.Next(1, 100);
                            var birthDay = randomGenerator.Next(10, 25);
                            var birthMonth = randomGenerator.Next(10, 12);
                            var birthYear = randomGenerator.Next(1980, 1998);
                            var wishName = GenerateFullName() + randomGenerator.Next(1, 99).ToString();
                            var password = GenerateFirstName() + randomGenerator.Next(1, 1234).ToString();
                            var resolvedCaptcha = CrackCaptcha(client, codeID);

                            var pairs = new List<KeyValuePair<string, string>>
                            {
                                new KeyValuePair<string, string>("do", "createAccount"),
                                new KeyValuePair<string, string>("transPostVars", "true"),
                                new KeyValuePair<string, string>("codeID", codeID),
                                new KeyValuePair<string, string>("salutation", "herr"),
                                new KeyValuePair<string, string>("email_local", wishName),
                                new KeyValuePair<string, string>("email_domain", "eclipso.de"),
                                new KeyValuePair<string, string>("firstname", firstName),
                                new KeyValuePair<string, string>("surname", lastName),
                                new KeyValuePair<string, string>("street", roadName),
                                new KeyValuePair<string, string>("no", roadNumber.ToString()),
                                new KeyValuePair<string, string>("zip", "10115"),
                                new KeyValuePair<string, string>("city", "Berlin"),
                                new KeyValuePair<string, string>("country", "25"),
                                new KeyValuePair<string, string>("field_5Day", birthDay.ToString()),
                                new KeyValuePair<string, string>("field_5Month", birthMonth.ToString()),
                                new KeyValuePair<string, string>("field_5Year", birthYear.ToString()),
                                new KeyValuePair<string, string>("field_3", "Google"),
                                new KeyValuePair<string, string>("field_4", "quasigaming"),
                                new KeyValuePair<string, string>("altmail", GenerateFullName() + "@" + GenerateLastName() + ".com"),
                                new KeyValuePair<string, string>("pass1", password),
                                new KeyValuePair<string, string>("pass2", password),
                                new KeyValuePair<string, string>("code", ""),
                                new KeyValuePair<string, string>("safecode", resolvedCaptcha),
                                new KeyValuePair<string, string>("tos", "true"),
                            };

                            var postData = new FormUrlEncodedContent(pairs);

                            if (resolvedCaptcha != "~")
                            {
                                var response = client.PostAsync(new Uri("https://www.eclipso.de/email-konto-anlegen.php"), postData).Result;
                                var responseContent = response.Content.ReadAsStringAsync().Result;
                                //System.IO.File.WriteAllText(@"ResposeCREATE.html", responseContent);

                                if (responseContent.Contains("mit Ihrer neuen"))
                                {
                                    //Login to activate account
                                    var loginDataPairs = new List<KeyValuePair<string, string>>
                                    {
                                        new KeyValuePair<string, string>("do", "login"),
                                        new KeyValuePair<string, string>("timezone", "7200"),
                                        new KeyValuePair<string, string>("email_full", wishName + "@eclipso.de"),
                                        new KeyValuePair<string, string>("password", password),
                                    };

                                    var loginPostData = new FormUrlEncodedContent(loginDataPairs);
                                    var loginResponse = client.PostAsync(new Uri("https://www.eclipso.de/login.php"), loginPostData).Result;
                                    var loginResponseContent = loginResponse.Content.ReadAsStringAsync().Result;
                                    //System.IO.File.WriteAllText(@"ResposeLOGINCREATE.html", loginResponseContent);

                                    ++proxyCount;
                                    Console.WriteLine(wishName + "@eclipso.de:" + password);

                                    System.IO.File.AppendAllText("EMails.txt", wishName + "@eclipso.de:" + password + "\n");
                                    created = true;
                                }

                                if (response.StatusCode == HttpStatusCode.NotFound)
                                    ++proxyCount;

                                //Console.ReadKey();
                            }

                            //Console.Write(".");
                        }
                    }
                    catch (Exception e)
                    {
                        //Console.WriteLine("Proxy Error: " + e.Message);
                        ++proxyCount;
                    }

                    if (!created)
                        --i;
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("LOL:" + e.Message);
            }
        }

        private static void Main(string[] args)
        {
            Console.Write("How many E-Mails should I create? ");
            int accountCount = Console.Read();

            //outputFile = System.IO.File.Create("EMails.txt");

            string[] proxyList = System.IO.File.ReadAllLines("ProxyList.txt");

            CreateAccounts(accountCount, proxyList);
            //int proxySplitCount = proxyList.Length / accountCount;
            //int lastSplit = 0;
            //Task[] tasks = new Task[accountCount];

            //for (int i = 0; i < accountCount; ++i)
            //{
            //    string[] tmpProxyList = new string[proxySplitCount];

            //    for (int j = 0; j < proxySplitCount; ++j)
            //        tmpProxyList[j] = proxyList[lastSplit + j];

            //    lastSplit += proxySplitCount;

            //    tasks[i] = Task.Run(async () => await CreateAccounts(1, tmpProxyList));
            //}

            //Task.WaitAll(tasks);
        }
    }
}