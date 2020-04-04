using System.Linq;

namespace MakeVirtualFileSystem.Utility
{
    internal class CommandLine : ICommandLine
    {
        private readonly string[] _commandLine;

        public CommandLine(string[] cmd)
        {
            _commandLine = cmd;
        }

        public bool Has(string key)
        {
            var keyname = BuildKeyForValueQuery(key);
            var foundKeyname = _commandLine.FirstOrDefault(s => s.StartsWith(keyname));
            if (string.IsNullOrWhiteSpace(foundKeyname))
                return false;

            var colonIndex = foundKeyname.IndexOf(':');
            if (colonIndex < 0)
                return false;

            return true;
        }

        public string Value(string key)
        {
            var keyname = BuildKeyForValueQuery(key);
            var foundKeyname = _commandLine.FirstOrDefault(s => s.StartsWith(keyname));
            if(string.IsNullOrWhiteSpace(foundKeyname))
                return string.Empty;

            var colonIndex = foundKeyname.IndexOf(':');
            if (colonIndex < 0)
                return string.Empty;

            var outputPath = foundKeyname.Substring(colonIndex + 1);
            return outputPath;
        }

        private static string BuildKeyForValueQuery(string key)
        {
            return $"--{key}";
        }

        private static string ExtractSubstring(string inputStr, char beginChar, char endChar)
        {
            var startIndex = inputStr.IndexOf(beginChar) + 1;
            if (startIndex == 0)
                return null;

            var endIndex = inputStr.IndexOf(endChar, startIndex);
            if (endIndex == -1)
                return null;

            var result = inputStr.Substring(startIndex, endIndex - startIndex);
            return result;
        }
    }
}
