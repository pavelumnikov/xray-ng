using MakeVirtualFileSystem.Extensions;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace MakeVirtualFileSystem.VFS.Internal
{
    [StructLayout(LayoutKind.Sequential)]
    internal class TableHeaderEntry
    {
        public static TableHeaderEntry Default => new TableHeaderEntry();

        public long FileCount => Files.Count;
        public long TableLength => VirtualEntriesFilePartEntry.Default.SizeOf * Files.Count;
        public byte[] TableShaHash { get; }

        public ICollection<VirtualEntriesFilePartEntry> Files { get; }

        public TableHeaderEntry()
        {
            TableShaHash = new byte[64];
            Files = new List<VirtualEntriesFilePartEntry>();
        }

        public long SizeOf => TableLength + FileCount.SizeOf() + TableLength.SizeOf() + TableShaHash.SizeOf();
    }
}
