using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace LAC
{
    internal struct PostData
    {
        public string Name;
        public string Value;
    }

    public class WebUser : IDisposable
    {
        public CookieContainer ck;
        private List<PostData> postDatas;
        private string cookie;

        public WebUser()
        {
            ck = new CookieContainer();
            postDatas = new List<PostData>();
        }

        public void Dispose()
        {
            postDatas.Clear();
        }

        public void Add(string attr, string val)
        {
            var postData = new PostData() { Name = attr, Value = val };
            postDatas.Add(postData);
        }

        public async Task<string> RequestString(string url, string referer = null, string host = null)
        {
            var ret = await RequestBytes(url, referer, host);
            if (ret == null)
                return null;
            return Encoding.UTF8.GetString(ret);
        }

        public async Task<byte[]> RequestBytes(string url, string referer = null, string host = null)
        {
            var request = (HttpWebRequest)HttpWebRequest.Create(url);
            request.CookieContainer = ck;
            request.Host = host;
            request.UserAgent = "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:25.0; WUID=d716f1e9f060b591542e7f5bdcd2ebd5; WTB=8086) Gecko/20100101 Firefox/25.0";

            request.ContentType = "application/x-www-form-urlencoded; charset=UTF-8";
            request.Timeout = 30 * 1000;

            if (postDatas.Count > 0)
            {
                request.Method = WebRequestMethods.Http.Post;
                request.Accept = "application/json, text/javascript, */*; q=0.01";
                //request.Connection = "keep-alive";
                request.KeepAlive = true;

                request.Headers.Add("Accept-Language", "de,en-US;q=0.7,en;q=0.3");
                request.Headers.Add("Accept-Encoding", "gzip, deflate");
                request.Headers.Add("X-Requested-With", "XMLHttpRequest");
                request.Headers.Add("Pragma", "no-cache");
                request.Headers.Add("Cache-Control", "no-cache");

                var first = true;
                var sb = new StringBuilder();
                foreach (var data in postDatas)
                {
                    sb.Append((first ? "" : "&") + System.Net.WebUtility.UrlEncode(data.Name) + "=" + System.Net.WebUtility.UrlEncode(data.Value));
                    //sb.Append((first ? "" : "&") + data.Name + "=" + data.Value);
                    if (first)
                        first = false;
                }

                var post = Encoding.UTF8.GetBytes(sb.ToString());
                request.ContentLength = post.Length;
                await request.GetRequestStream().WriteAsync(post, 0, post.Length);
                postDatas.Clear();
            }
            else
                request.Method = WebRequestMethods.Http.Get;

            if (!String.IsNullOrEmpty(referer))
                request.Referer = referer;

            try
            {
                using (var response = await request.GetResponseAsync())
                using (var stream = response.GetResponseStream())
                using (var memStream = new MemoryStream())
                {
                    foreach (Cookie cookieValue in ((HttpWebResponse)response).Cookies)
                    {
                        cookie = cookieValue.ToString();
                        Console.WriteLine("Cookie: " + cookie);
                    }

                    await stream.CopyToAsync(memStream);
                    return memStream.ToArray();
                }
            }
            catch
            {
                return null;
            }
        }
    }
}