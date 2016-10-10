using System;
using System.Collections;
using System.Collections.Generic;
using System.Dynamic;
using System.Globalization;
using System.IO;

namespace BlubLib.Configuration
{
    public class Config : DynamicObject, IEnumerable<KeyValuePair<string, ConfigSection>>
    {
        private readonly Dictionary<string, ConfigSection> _dictionary = new Dictionary<string, ConfigSection>();

        private string _filePath;

        public ICollection<ConfigSection> Sections
        {
            get { return _dictionary.Values; }
        }

        public ConfigSection this[string key]
        {
            get { return GetSection(key); }
        }

        public static Config Load(string fileName)
        {
            return Load(fileName, new INISerializer());
        }

        public static Config Load(string fileName, IConfigSerializer serializer)
        {
            Config config;

            if (File.Exists(fileName))
            {
                using (var fs = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.Read))
                    config = serializer.Deserialize(fs);
            }
            else
            {
                config = new Config();
            }

            config._filePath = fileName;
            return config;
        }

        public static Config Load(Stream stream)
        {
            return Load(stream, new INISerializer());
        }

        public static Config Load(Stream stream, IConfigSerializer serializer)
        {
            Config config;

            using (stream)
                config = serializer.Deserialize(stream);

            return config;
        }

        public void Save(string fileName)
        {
            Save(fileName, new INISerializer());
        }

        public void Save(string fileName, IConfigSerializer serializer)
        {
            using (var fs = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.None))
                serializer.Serialize(fs, this);
        }

        public void Save()
        {
            if (!string.IsNullOrWhiteSpace(_filePath))
                Save(_filePath);
        }

        public void Save(IConfigSerializer serializer)
        {
            if (!string.IsNullOrWhiteSpace(_filePath))
                Save(_filePath, serializer);
        }

        public ConfigSection GetSection(string key)
        {
            if (_dictionary.ContainsKey(key))
                return _dictionary[key];

            var value = new ConfigSection(key);
            _dictionary.Add(key, value);
            return value;
        }

        #region DynamicObject

        public override bool TryGetMember(GetMemberBinder binder, out object result)
        {
            result = GetSection(binder.Name);
            return true;
        }

        #endregion DynamicObject

        #region IEnumerator

        public IEnumerator<KeyValuePair<string, ConfigSection>> GetEnumerator()
        {
            return _dictionary.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _dictionary.GetEnumerator();
        }

        #endregion IEnumerator
    }

    public class ConfigSection : DynamicObject, IEnumerable<KeyValuePair<string, ConfigValue>>
    {
        private readonly Dictionary<string, ConfigValue> _dictionary = new Dictionary<string, ConfigValue>();

        public ICollection<ConfigValue> Values
        {
            get { return _dictionary.Values; }
        }

        public string Name { get; private set; }

        public ConfigSection(string name)
        {
            Name = name;
        }

        public ConfigValue this[string key]
        {
            get { return GetValue(key); }
            set { SetValue(key, value); }
        }

        public ConfigValue GetValue(string key)
        {
            if (_dictionary.ContainsKey(key))
                return _dictionary[key];

            var value = new ConfigValue();
            _dictionary.Add(key, value);
            return value;
        }

        public void SetValue(string key, ConfigValue value)
        {
            if (_dictionary.ContainsKey(key))
                _dictionary[key] = value;
            else
                _dictionary.Add(key, value);
        }

        #region DynamicObject

        public override bool TryGetMember(GetMemberBinder binder, out object result)
        {
            result = GetValue(binder.Name);
            return true;
        }

        public override bool TrySetMember(SetMemberBinder binder, object value)
        {
            if (!(value is ConfigValue))
                SetValue(binder.Name, new ConfigValue(value.ToString()));
            else
                SetValue(binder.Name, (ConfigValue)value);
            return true;
        }

        #endregion DynamicObject

        #region IEnumerator

        public IEnumerator<KeyValuePair<string, ConfigValue>> GetEnumerator()
        {
            return _dictionary.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _dictionary.GetEnumerator();
        }

        #endregion IEnumerator

        public override string ToString()
        {
            return Name;
        }
    }

    public class ConfigValue : IComparable, IConvertible, IFormattable
    {
        public string Value { get; set; }

        public ConfigValue()
        {
            Value = "";
        }

        public ConfigValue(string value)
        {
            Value = value;
        }

        #region implicit operators ConfigValue -> x

        public static implicit operator string(ConfigValue value)
        {
            return value.Value;
        }

        public static implicit operator char(ConfigValue value)
        {
            return value.ToChar(CultureInfo.InvariantCulture);
        }

        public static implicit operator byte(ConfigValue value)
        {
            return value.ToByte(CultureInfo.InvariantCulture);
        }

        public static implicit operator bool(ConfigValue value)
        {
            return value.ToBoolean(CultureInfo.InvariantCulture);
        }

        public static implicit operator Int16(ConfigValue value)
        {
            return value.ToInt16(CultureInfo.InvariantCulture);
        }

        public static implicit operator Int32(ConfigValue value)
        {
            return value.ToInt32(CultureInfo.InvariantCulture);
        }

        public static implicit operator Int64(ConfigValue value)
        {
            return value.ToInt64(CultureInfo.InvariantCulture);
        }

        public static implicit operator UInt16(ConfigValue value)
        {
            return value.ToUInt16(CultureInfo.InvariantCulture);
        }

        public static implicit operator UInt32(ConfigValue value)
        {
            return value.ToUInt32(CultureInfo.InvariantCulture);
        }

        public static implicit operator UInt64(ConfigValue value)
        {
            return value.ToUInt64(CultureInfo.InvariantCulture);
        }

        public static implicit operator Single(ConfigValue value)
        {
            return value.ToSingle(CultureInfo.InvariantCulture);
        }

        public static implicit operator Double(ConfigValue value)
        {
            return value.ToDouble(CultureInfo.InvariantCulture);
        }

        #endregion implicit operators ConfigValue -> x

        #region implicit operators x -> ConfigValue

        public static implicit operator ConfigValue(string value)
        {
            return new ConfigValue(value);
        }

        public static implicit operator ConfigValue(char value)
        {
            return new ConfigValue(value.ToString(CultureInfo.InvariantCulture));
        }

        public static implicit operator ConfigValue(byte value)
        {
            return new ConfigValue(value.ToString(CultureInfo.InvariantCulture));
        }

        public static implicit operator ConfigValue(bool value)
        {
            return new ConfigValue(value.ToString(CultureInfo.InvariantCulture));
        }

        public static implicit operator ConfigValue(Int16 value)
        {
            return new ConfigValue(value.ToString(CultureInfo.InvariantCulture));
        }

        public static implicit operator ConfigValue(Int32 value)
        {
            return new ConfigValue(value.ToString(CultureInfo.InvariantCulture));
        }

        public static implicit operator ConfigValue(Int64 value)
        {
            return new ConfigValue(value.ToString(CultureInfo.InvariantCulture));
        }

        public static implicit operator ConfigValue(UInt16 value)
        {
            return new ConfigValue(value.ToString(CultureInfo.InvariantCulture));
        }

        public static implicit operator ConfigValue(UInt32 value)
        {
            return new ConfigValue(value.ToString(CultureInfo.InvariantCulture));
        }

        public static implicit operator ConfigValue(UInt64 value)
        {
            return new ConfigValue(value.ToString(CultureInfo.InvariantCulture));
        }

        public static implicit operator ConfigValue(Single value)
        {
            return new ConfigValue(value.ToString(CultureInfo.InvariantCulture));
        }

        public static implicit operator ConfigValue(Double value)
        {
            return new ConfigValue(value.ToString(CultureInfo.InvariantCulture));
        }

        #endregion implicit operators x -> ConfigValue

        public override string ToString()
        {
            return Value;
        }

        public string ToString(string format, IFormatProvider formatProvider)
        {
            return Value;
        }

        public int CompareTo(object obj)
        {
            var val = Convert.ChangeType(Value, obj.GetType());
            return Comparer.DefaultInvariant.Compare(val, obj);
        }

        #region IConvertible

        public TypeCode GetTypeCode()
        {
            return TypeCode.Object;
        }

        public bool ToBoolean(IFormatProvider provider)
        {
            bool val;
            return bool.TryParse(Value, out val) && val;
        }

        public char ToChar(IFormatProvider provider)
        {
            char val;
            return char.TryParse(Value, out val) ? val : ' ';
        }

        public sbyte ToSByte(IFormatProvider provider)
        {
            sbyte val;
            return (sbyte)(sbyte.TryParse(Value, out val) ? val : 0);
        }

        public byte ToByte(IFormatProvider provider)
        {
            byte val;
            return (byte)(byte.TryParse(Value, out val) ? val : 0);
        }

        public short ToInt16(IFormatProvider provider)
        {
            Int16 val;
            return (Int16)(Int16.TryParse(Value, out val) ? val : 0);
        }

        public ushort ToUInt16(IFormatProvider provider)
        {
            UInt16 val;
            return (UInt16)(UInt16.TryParse(Value, out val) ? val : 0);
        }

        public int ToInt32(IFormatProvider provider)
        {
            Int32 val;
            return (Int32.TryParse(Value, out val) ? val : 0);
        }

        public uint ToUInt32(IFormatProvider provider)
        {
            UInt32 val;
            return (UInt32.TryParse(Value, out val) ? val : 0);
        }

        public long ToInt64(IFormatProvider provider)
        {
            Int64 val;
            return (Int64.TryParse(Value, out val) ? val : 0);
        }

        public ulong ToUInt64(IFormatProvider provider)
        {
            UInt64 val;
            return (UInt64.TryParse(Value, out val) ? val : 0);
        }

        public float ToSingle(IFormatProvider provider)
        {
            Single val;
            return (Single.TryParse(Value, out val) ? val : 0);
        }

        public double ToDouble(IFormatProvider provider)
        {
            Double val;
            return (Double.TryParse(Value, out val) ? val : 0);
        }

        public decimal ToDecimal(IFormatProvider provider)
        {
            Decimal val;
            return (Decimal.TryParse(Value, out val) ? val : 0);
        }

        public DateTime ToDateTime(IFormatProvider provider)
        {
            throw new NotSupportedException();
        }

        public string ToString(IFormatProvider provider)
        {
            return Value;
        }

        public object ToType(Type conversionType, IFormatProvider provider)
        {
            return Convert.ChangeType(Value, conversionType);
        }

        #endregion IConvertible
    }
}