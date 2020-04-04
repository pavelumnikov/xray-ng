using MakeVirtualFileSystem.Extensions;
using System.Runtime.InteropServices;

namespace MakeVirtualFileSystem.VFS.Internal
{
    [StructLayout(LayoutKind.Sequential)]
    internal class VirtualEntriesFilePartEntry
    {
        public static VirtualEntriesFilePartEntry Default => new VirtualEntriesFilePartEntry();

        public long Offset { get; set; }
        public long Bytes { get; set; }
        public byte[] FileName { get; set; }
        public byte[] Hash { get; set; }

        public VirtualEntriesFilePartEntry()
        {
            FileName = new byte[256];
            Hash = new byte[64];
        }

        public long SizeOf => Offset.SizeOf() + Bytes.SizeOf() + FileName.SizeOf() + Hash.SizeOf();
    }
}
