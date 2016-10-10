using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace BlubLib.Configuration
{
    public interface IConfigSerializer
    {
        Config Deserialize(Stream stream);

        void Serialize(Stream stream, Config config);
    }

    public class INISerializer : IConfigSerializer
    {
        private static readonly Regex _sectionRegex = new Regex(@"^\[([a-zA-Z0-9_-]+)\]");
        private static readonly Regex _valueRegex = new Regex(@"^([a-zA-Z0-9.,_-]+)=(.*)");

        public Config Deserialize(Stream stream)
        {
            var config = new Config();

            using (var r = new StreamReader(stream))
            {
                string line;
                string lastSection = null;
                while ((line = r.ReadLine()) != null)
                {
                    if (string.IsNullOrWhiteSpace(line)) continue;
                    if (_sectionRegex.IsMatch(line))
                    {
                        var match = _sectionRegex.Match(line);
                        var name = match.Groups[1].Value;
                        lastSection = name;
                        config.GetSection(name);
                    }
                    else if (_valueRegex.IsMatch(line) && lastSection != null)
                    {
                        var match = _valueRegex.Match(line);
                        config[lastSection][match.Groups[1].Value] = match.Groups[2].Value;
                    }
                }
            }

            return config;
        }

        public void Serialize(Stream stream, Config config)
        {
            var sb = new StringBuilder();
            foreach (var pair in config)
            {
                sb.AppendLine("[" + pair.Key + "]");
                foreach (var value in pair.Value)
                    sb.AppendLine(value.Key + "=" + value.Value);

                sb.AppendLine("");
            }
            using (var w = new StreamWriter(stream))
                w.Write(sb.ToString());
        }
    }
}