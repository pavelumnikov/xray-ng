using MakeVirtualFileSystem.Extensions;
using MakeVirtualFileSystem.VFS.Extensions;
using MakeVirtualFileSystem.VFS.Internal;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Threading;
using System.Threading.Tasks;

namespace MakeVirtualFileSystem.VFS
{
    internal class PackFileProcess : IFileProcess
    {
        private readonly Utility.ILoggingFacility _loggingFacility;
        private readonly IList<string> _filesPaths; // this has full path
        private volatile bool _isDone = false;

        public PackFileProcess(Utility.ILoggingFacility loggingFacility)
        {
            _loggingFacility = loggingFacility;
            _filesPaths = new List<string>();
        }

        public void Begin(string basePath, string packFile)
        {
            Task.Factory.StartNew(() => TaskFunction(basePath, packFile), TaskCreationOptions.LongRunning);
        }

        private async void TaskFunction(string basePath, string packFile)
        {
            var filesPaths = await Task.Factory.StartNew(() => DirSearchAction(basePath));
            var vpk = await CollectFilesForVpk(packFile, filesPaths);

            vpk.CreateFileOnDisk(_loggingFacility);

            var firstPass = Task.Factory.StartNew(() => FirstPassVpk(vpk));
            var secondPass = Task.Factory.StartNew(() => SecondPassVpk(vpk));
            await Task.WhenAll(firstPass, secondPass);

            _isDone = true;
        }

        private async Task<VpkFileEntry> CollectFilesForVpk(string packFile, ICollection<string> files)
        {
            var result = new VpkFileEntry(packFile);

            await Task.Factory.StartNew(() =>
            {
                long sizeFromTableEnd = 0;
                foreach (var f in files)
                {
                    var fileInfo = new FileInfo(f);
                    var fileDescriptor = new VirtualEntriesFilePartEntry
                    {
                        FileName = f.ToByteArray()
                    };

                    fileDescriptor.Bytes = fileInfo.Length;
                    fileDescriptor.Offset = sizeFromTableEnd;

                    sizeFromTableEnd += fileDescriptor.Bytes;

                    result.Table.Files.Add(fileDescriptor);
                }
            });

            var taskCalculateSha512ForFiles = Task.Factory.StartNew(() =>
            {
                foreach (var fileDescriptor in result.Table.Files)
                    fileDescriptor.CalculateSHA512();
            });

            var taskFixupFileOffsets = Task.Factory.StartNew(() => result.FixupOffsetForFiles());
            var taskCalculateSha512 = Task.Factory.StartNew(() => result.CalculateSHA512());
            await Task.WhenAll(taskCalculateSha512ForFiles, taskFixupFileOffsets, taskCalculateSha512);

            return result;
        }

        private void FirstPassVpk(VpkFileEntry vpkFile)
        {
            using (var fileStream = File.Open(vpkFile.FileName, FileMode.Open, FileAccess.ReadWrite, FileShare.ReadWrite))
            {
                var mappedFileName = MakeMappedFileNameForTask("createVpkMapFirstParallelPass");
                var memorySecurity = CreateMemoryMappedFileSecurity();
                using (var mmf = MemoryMappedFile.CreateFromFile(fileStream, mappedFileName, 0, 
                    MemoryMappedFileAccess.ReadWrite, memorySecurity, HandleInheritability.Inheritable, false))
                {
                    // First pass: preload table with file data
                    var firstPassBaseOffset = 0;
                    using (var accessor = mmf.CreateViewAccessor(firstPassBaseOffset, vpkFile.Table.SizeOf))
                    {
                        long position = 0;
                        accessor.Write(0, vpkFile.Table.FileCount);
                        position += vpkFile.Table.FileCount.SizeOf();

                        // write table size in bytes
                        accessor.Write(position, vpkFile.Table.TableLength);
                        position += vpkFile.Table.TableLength.SizeOf();

                        // write table SHA512 hash
                        accessor.WriteArray(position, vpkFile.Table.TableShaHash, 0, vpkFile.Table.TableShaHash.Length);
                        position += vpkFile.Table.TableShaHash.SizeOf();

                        foreach (var file in vpkFile.Table.Files)
                        {
                            accessor.Write(position, file.Offset);
                            position += file.Offset.SizeOf();

                            accessor.Write(position, file.Bytes);
                            position += file.Bytes.SizeOf();

                            accessor.WriteArray(position, file.FileName, 0, file.FileName.Length);
                            position += file.FileName.SizeOf();

                            _loggingFacility.LogFromThread($"VFS table entry: offset:'{file.Offset}'; size:'{file.Bytes}'; filename:'{file.FileName.FromByteArray()}'\r\n");
                        }

                        Console.WriteLine($"VFS file table size: via position:'{position}'; via sizeof:'{vpkFile.Table.SizeOf}'");
                    }
                }
            }
        }

        private void SecondPassVpk(VpkFileEntry vpkFile)
        {
            using (var fileStream = File.Open(vpkFile.FileName, FileMode.Open, FileAccess.ReadWrite, FileShare.ReadWrite))
            {
                var mappedFileName = MakeMappedFileNameForTask("createVpkMapSecondParallelPass");
                var memorySecurity = CreateMemoryMappedFileSecurity();
                using (var mmf = MemoryMappedFile.CreateFromFile(fileStream, mappedFileName, 0,
                    MemoryMappedFileAccess.ReadWrite, memorySecurity, HandleInheritability.Inheritable, false))
                {
                    using (var accessor = mmf.CreateViewAccessor(vpkFile.Table.SizeOf, vpkFile.FilesBytes))
                    {
                        const int bytesToReadDefault = 65535; // 65 kilobytes

                        var writePosition = 0;
                        foreach (var file in vpkFile.Table.Files)
                        {
                            int readPosition = 0;
                            using (var fileToReadFrom = File.OpenRead(file.FileName.FromByteArray()))
                            {
                                BinaryReader binaryReader = new BinaryReader(fileToReadFrom);
                                _loggingFacility.LogFromThread($"VFS file entry: filename:'{file.FileName.FromByteArray()}'; size:'{file.Bytes}'\r\n");

                                while (readPosition < fileToReadFrom.Length)
                                {
                                    int actualSizeRead = (int)fileToReadFrom.Length - readPosition;
                                    if (actualSizeRead >= bytesToReadDefault)
                                        actualSizeRead = bytesToReadDefault;

                                    var tempBuffer = binaryReader.ReadBytes(actualSizeRead);
                                    accessor.WriteArray(writePosition, tempBuffer, 0, actualSizeRead);

                                    _loggingFacility.LogFromThread($"Copying from:'{file.FileName.FromByteArray()}' '{actualSizeRead}' bytes to VPK\r\n");

                                    readPosition += actualSizeRead;
                                    writePosition += actualSizeRead;
                                }
                            }
                        }
                    }
                }
            }
        }

        private ICollection<string> DirSearchAction(string sDir)
        {
            var result = new List<string>();
            //DirSearchCurrentInternal(sDir, result);
            DirSearchInternal(sDir, result);
            return result;
        }

        private void DirSearchCurrentInternal(string sDir, ICollection<string> fileCollection)
        {
            try
            {
                foreach (var f in Directory.GetFiles(sDir))
                {
                    fileCollection.Add(f);
                }

            }
            catch (Exception except)
            {
                _loggingFacility.LogFromThread(except.Message);
            }
        }

        private void DirSearchInternal(string sDir, ICollection<string> fileCollection)
        {
            try
            {
                var directories = Directory.GetDirectories(sDir);
                foreach (var d in directories)
                {
                    var files = Directory.GetFiles(d);
                    foreach (var f in files)
                    {
                        fileCollection.Add(f);
                    }

                    DirSearchInternal(d, fileCollection);
                }
            }
            catch (Exception except)
            {
                _loggingFacility.LogFromThread(except.Message);
            }
        }

        public bool IsDone()
        {
            return _isDone;
        }

        private string MakeMappedFileNameForTask(string baseName)
        {
            return $"{baseName}_forThreadTask{Thread.CurrentThread.ManagedThreadId}";
        }

        private MemoryMappedFileSecurity CreateMemoryMappedFileSecurity()
        {
            return new MemoryMappedFileSecurity();
        }
    }
}
