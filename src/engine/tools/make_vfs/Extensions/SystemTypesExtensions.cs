namespace MakeVirtualFileSystem.Extensions
{
    internal static class SystemTypesExtensions
    {
        public static long SizeOf(this long type)
        {
            return sizeof(long);
        }

        public static long SizeOf(this ulong type)
        {
            return sizeof(ulong);
        }

        public static long SizeOf(this byte[] type)
        {
            return sizeof(byte) * type.Length;
        }
    }
}
