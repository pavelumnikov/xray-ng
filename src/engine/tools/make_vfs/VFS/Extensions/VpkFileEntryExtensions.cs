using MakeVirtualFileSystem.Utility;
using MakeVirtualFileSystem.VFS.Internal;
using System;
using System.IO;
using System.Security.Cryptography;

namespace MakeVirtualFileSystem.VFS.Extensions
{
    internal static class VpkFileEntryExtensions
    {
        public static void CreateFileOnDisk(this VpkFileEntry self, ILoggingFacility loggingFacility)
        {
            if (File.Exists(self.FileName))
                File.Delete(self.FileName);

            using (var filestream = File.Create(self.FileName))
                filestream.SetLength(self.VpkFileSize);

            var length = new FileInfo(self.FileName).Length;
            if (length != self.VpkFileSize)
                throw new UnauthorizedAccessException($"File size: {self.VpkFileSize}; Expection: {length}");

            loggingFacility.LogFromThread($"Created file: '{self.FileName}'");
        }

        public static void FixupOffsetForFiles(this VpkFileEntry self)
        {
            var baseOffset = self.Table.SizeOf;
            foreach (var fileEntry in self.Table.Files)
            {
                fileEntry.Offset += baseOffset;
            }
        }

        public static void CalculateSHA512(this VpkFileEntry self)
        {
            using (var sha512 = SHA512.Create())
            {
                using (var memoryStream = new MemoryStream())
                {
                    using (var binaryWriter = new BinaryWriter(memoryStream))
                    {
                        binaryWriter.Write(self.FileName); // first we take filename
                        binaryWriter.Write(self.FilesBytes); // then we take bytes of files that are inside
                        binaryWriter.Write(self.VpkFileSize); // then VPK file size in bytes
                        var hash = sha512.ComputeHash(memoryStream);
                        hash.CopyTo(self.Table.TableShaHash, 0);
                    }
                }
            }
        }
    }
}
