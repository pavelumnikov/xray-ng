using System.Text;

namespace MakeVirtualFileSystem.Extensions
{
    internal static class StringExtensions
    {
        public static byte[] ToByteArray(this string str)
        {
            return Encoding.ASCII.GetBytes(str);
        }
    }
}
