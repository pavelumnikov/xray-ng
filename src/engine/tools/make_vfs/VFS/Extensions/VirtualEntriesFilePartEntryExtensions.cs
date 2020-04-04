using MakeVirtualFileSystem.Extensions;
using MakeVirtualFileSystem.VFS.Internal;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;

namespace MakeVirtualFileSystem.VFS.Extensions
{
    internal static class VirtualEntriesFilePartEntryExtensions
    {
        public static long FileSummarySize(this ICollection<VirtualEntriesFilePartEntry> self)
        {
            long result = 0;
            foreach(var file in self)
            {
                result += file.Bytes;
            }

            return result;
        }

        public static void CalculateSHA512(this VirtualEntriesFilePartEntry self)
        {
            using (var sha512 = SHA512.Create())
            {
                using (var memoryStream = new MemoryStream())
                {
                    using (var binaryWriter = new BinaryWriter(memoryStream))
                    {
                        binaryWriter.Write(self.FileName); // first we take filename
                        binaryWriter.Write(self.Bytes); // then we take bytes of files that are inside
                        binaryWriter.Write(self.SizeOf); // then VPK file size in bytes

                        using (var fileStream = File.OpenRead(self.FileName.FromByteArray()))
                        {
                            using (var binaryReader = new BinaryReader(fileStream))
                            {
                                var bytes = binaryReader.ReadBytes(8);
                                binaryWriter.Write(bytes);
                            }
                        }

                        var hash = sha512.ComputeHash(memoryStream);
                        hash.CopyTo(self.Hash, 0);
                    }
                }
            }
        }
    }
}
