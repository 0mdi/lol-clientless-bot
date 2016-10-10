using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Net.Http;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;

namespace LoLCaptcha
{
    internal class Program
    {
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

        static public void TurnBlack(ref Bitmap captcha)
        {
            Color OldColor;

            for (int i = 0; i < captcha.Width; i++)
            {
                for (int k = 0; k < captcha.Height; k++)
                {
                    if (captcha.GetPixel(i, k).Name == "ffe3daed")
                    {
                        OldColor = captcha.GetPixel(i, k);
                        captcha.SetPixel(i, k, Color.White);
                    }
                    else if (captcha.GetPixel(i, k).Name == "ff80bfff")
                    {
                        OldColor = captcha.GetPixel(i, k);
                        captcha.SetPixel(i, k, Color.White);
                    }
                    else if (captcha.GetPixel(i, k).Name == "fff36e5f")
                    {
                        OldColor = captcha.GetPixel(i, k);
                    }
                    else if (captcha.GetPixel(i, k).Name == "ff8080ff" && k > 0)
                    {
                        OldColor = captcha.GetPixel(i, k - 1);
                        captcha.SetPixel(i, k, OldColor);
                    }
                }
            }

            for (int x = 0; x < captcha.Width; x++)
            {
                for (int y = 0; y < captcha.Height; y++)
                {
                    if (captcha.GetPixel(x, y).Name != "ffffffff")
                    {
                        FloodFill(captcha, x, y, Color.Black);
                    }
                }
            }

            for (int i = 0; i < captcha.Width; i++)
            {
                for (int k = 0; k < captcha.Height; k++)
                {
                    if (captcha.GetPixel(i, k).Name == "ffe3daed")
                    {
                        OldColor = captcha.GetPixel(i, k);
                        captcha.SetPixel(i, k, Color.White);
                    }
                    else if (captcha.GetPixel(i, k).Name == "ff80bfff")
                    {
                        OldColor = captcha.GetPixel(i, k);
                        captcha.SetPixel(i, k, Color.White);
                    }
                    else if (captcha.GetPixel(i, k).Name == "fff36e5f")
                    {
                        OldColor = captcha.GetPixel(i, k);
                    }
                    else if (captcha.GetPixel(i, k).Name == "ff8080ff" && k > 0)
                    {
                        OldColor = captcha.GetPixel(i, k - 1);
                        captcha.SetPixel(i, k, OldColor);
                    }
                }
            }

            for (int x = 0; x < captcha.Width; x++)
            {
                for (int y = 0; y < captcha.Height; y++)
                {
                    if (captcha.GetPixel(x, y).Name != "ffffffff")
                    {
                        FloodFill(captcha, x, y, Color.Black);
                    }
                }
            }
        }

        static public void OverwriteBitmap(ref Bitmap origBitmap, Bitmap copyBitmap)
        {
            for (int i = 0; i < origBitmap.Width; ++i)
            {
                for (int j = 0; j < origBitmap.Height; ++j)
                {
                    origBitmap.SetPixel(i, j, copyBitmap.GetPixel(i, j));
                }
            }
        }

        static public bool HasBlack(Bitmap bitmap)
        {
            for (int i = 0; i < bitmap.Width; ++i)
            {
                for (int j = 0; j < bitmap.Height; ++j)
                {
                    if (bitmap.GetPixel(i, j).Name == "ff000000")
                        return true;
                }
            }

            return false;
        }

        static public void DiluteLetters(ref List<Bitmap> letterList)
        {
            for (int k = 0; k < letterList.Count; ++k)
            {
                Bitmap letter = letterList[k];
                Bitmap newLetter = new Bitmap(letter.Width, letter.Height);

                for (int x = 0; x < newLetter.Width; ++x)
                    for (int y = 0; y < newLetter.Height; ++y)
                        newLetter.SetPixel(x, y, Color.White);

                int widthParts = letter.Width / 4;
                int heightParts = letter.Height / 4;

                //Cut into pieces!
                for (int i = 0; i < widthParts; ++i)
                {
                    for (int j = 0; j < heightParts; ++j)
                    {
                        var rect = new Rectangle(new Point(i * 4, j * 4), new Size(4, 4));
                        var piece = letter.Clone(rect, letter.PixelFormat);

                        if (HasBlack(piece))
                        {
                            newLetter.SetPixel(i * 4 + 2, j * 4 + 2, Color.Black);
                        }
                    }
                }

                //Overwrite letter
                OverwriteBitmap(ref letter, newLetter);
            }
        }

        static public List<Bitmap> SeperateLetters(Bitmap captcha)
        {
            //captcha.Save("captcha2.png");

            List<Bitmap> letterList = new List<Bitmap>();

            int startWidthOffset = -1;
            int endWidthOffset = -1;
            int startHeightOffset = -1;

            bool searchStart = true;
            bool foundInColumn = false;

            for (int i = 0; i < captcha.Width; ++i)
            {
                for (int j = 0; j < captcha.Height; ++j)
                {
                    //Console.WriteLine(captcha.GetPixel(i, j).Name);
                    if (captcha.GetPixel(i, j).Name == "ff000000")
                    {
                        foundInColumn = true;

                        if (searchStart == true)
                        {
                            startWidthOffset = i;
                            startHeightOffset = j;

                            searchStart = false;
                        }
                    }
                }

                if (foundInColumn == false || i == captcha.Width - 1)
                {
                    if (startWidthOffset != -1)
                    {
                        endWidthOffset = i;

                        var rect = new Rectangle(new Point(startWidthOffset, 0), new Size(endWidthOffset - startWidthOffset, captcha.Height));
                        letterList.Add(captcha.Clone(rect, captcha.PixelFormat));

                        startWidthOffset = -1;
                        startHeightOffset = -1;
                        searchStart = true;
                    }
                }

                foundInColumn = false;
            }

            return letterList;
        }

        static public void RotateLetters(ref Bitmap captcha)
        {
            //Seperate letters
            var letterList = SeperateLetters(captcha);

            DiluteLetters(ref letterList);

            for (int i = 0; i < letterList.Count; ++i)
            {
                letterList[i].Save(i.ToString() + ".png");
            }
        }

        static public void PrepareCaptcha(Bitmap captcha)
        {
            TurnBlack(ref captcha);
            RotateLetters(ref captcha);
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

        private static async Task<string> CrackCaptcha(HttpClient client)
        {
            //System.Diagnostics.Debugger.Break();
            try
            {
                Bitmap captchaBitmap = await LoadPicture(client, "https://signup.leagueoflegends.com/de/signup/captcha/");
                PrepareCaptcha(captchaBitmap);
                captchaBitmap.Save("captcha.png");

                string result = "";

                using (tessnet2.Tesseract ocr = new tessnet2.Tesseract())
                {
                    ocr.Init(Directory.GetCurrentDirectory() + @"\tessdata\", "eng", false);
                    ocr.SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

                    List<tessnet2.Word> wlist = ocr.DoOCR(captchaBitmap, Rectangle.Empty);
                    foreach (tessnet2.Word w in wlist)
                        result += w.Text;

                    ocr.Clear();
                    ocr.Dispose();
                    wlist.Clear();

                    return result;
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }

            return "ERROR";
        }

        private static async void mainLoop()
        {
            HttpClient client = new HttpClient();

            while (true)
            {
                Console.WriteLine(await CrackCaptcha(client));
                Console.ReadLine();
            }
        }

        private static void Main(string[] args)
        {
            mainLoop();

            while (true)
                Thread.Yield();
        }
    }
}