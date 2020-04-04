using System.Text;

namespace MakeVirtualFileSystem.Extensions
{
    internal static class ByteArrayExtensions
    {
        public static string FromByteArray(this byte[] byteArray)
        {
            return Encoding.ASCII.GetString(byteArray);
        }
    }
}
