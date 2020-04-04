using MakeVirtualFileSystem.VFS.Extensions;
using System.Runtime.InteropServices;

namespace MakeVirtualFileSystem.VFS.Internal
{
    [StructLayout(LayoutKind.Sequential)]
    internal class VpkFileEntry
    {
        public string FileName { get; }
        public TableHeaderEntry Table { get; }
        public long FilesBytes => Table.Files.FileSummarySize();
        public long VpkFileSize => Table.SizeOf + FilesBytes;

        public VpkFileEntry(string filePath)
        {
            FileName = filePath;
            Table = new TableHeaderEntry();
        }
    }
}
