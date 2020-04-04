using System;
using System.Collections.Generic;
using System.IO;

namespace MakeVirtualFileSystem
{
    class Program
    {
        static void ShowHelp()
        {
            Console.WriteLine("--- Aztek Engine VFS tools ---\n");
            Console.WriteLine("--base:<path> - base path to pack files from or unpack files to [REQUIRED]");
            Console.WriteLine("--output-name:<name> - name of output file (wihtout .vfs extenions) [REQUIRED]");
            Console.WriteLine("--command:<command> - command name to process [REQUIRED]");
            Console.WriteLine("\tcreateVpk - creates new VPK file");
            Console.WriteLine("\tunpackVpk - unpacks already generated VPK file");
            Console.WriteLine("\tlistVpk - list files from already generated VPK file");
            Console.WriteLine("--log-path:<path> - output directory for log.txt file [OPTIONAL]");
            Console.WriteLine("--output-path:<path> - output directory [OPTIONAL]");

            Environment.Exit(1);
        }

        static void ExecuteOrShowHelp(Utility.ICommandLine commandLine, Utility.ILoggingFacility loggingFacility,
            Utility.ILoggingFacilityExecutor loggingFacilityExecutor)
        {
            var commandList = new Dictionary<string, Func<VFS.IFileProcess>>(2)
            {
                { "createVpk", new Func<VFS.IFileProcess>(() => new VFS.PackFileProcess(loggingFacility)) },
                { "unpackVpk", new Func<VFS.IFileProcess>(() => new VFS.UnpackFileProcess(loggingFacility)) },
                { "listVpk", new Func<VFS.IFileProcess>(() => new VFS.ListFileProcess(loggingFacility)) }
            };

            var outputPath = commandLine.Value("output-path");
            var outputBaseFilename = commandLine.Value("output-name");
            var commandToExecute = commandLine.Value("command");
            var baseDirectory = commandLine.Value("base");

            if (string.IsNullOrWhiteSpace(outputPath))
                outputPath = Directory.GetCurrentDirectory();

            var outputFilename = Path.Combine(outputPath, $"{outputBaseFilename}.vfs");
            Directory.SetCurrentDirectory(baseDirectory);

            if (string.IsNullOrWhiteSpace(commandToExecute) || !commandList.ContainsKey(commandToExecute))
                ShowHelp();

            var action = commandList[commandToExecute].Invoke();
            action.Begin(baseDirectory, outputFilename);
            loggingFacilityExecutor.WaitForMessagesUntil(() => !action.IsDone());
        }

        static void Main(string[] args)
        {
            Utility.ICommandLine commandLineParser = new Utility.CommandLine(args);
            var loggingDir = commandLineParser.Value("log-path");
            if (string.IsNullOrWhiteSpace(loggingDir))
            {
                loggingDir = Directory.GetCurrentDirectory();
            }

            var loggingFacilityImpl = new Utility.LoggingFacility(Path.Combine(loggingDir, "log.txt"));
            var loggingFacility = loggingFacilityImpl as Utility.ILoggingFacility;
            var loggingFacilityExecutor = loggingFacilityImpl as Utility.ILoggingFacilityExecutor;

            if (!commandLineParser.Has("output-name"))
            {
                loggingFacility.Log("Invalid filename for output vfs file");
                ShowHelp();
            }

            if (!commandLineParser.Has("base"))
            {
                loggingFacility.Log("Invalid name for base root path");
                ShowHelp();
            }

            ExecuteOrShowHelp(commandLineParser, loggingFacility, loggingFacilityExecutor);

#if USE_READ
            // Read all contents from Virtual File System

            // Create the memory-mapped file.
            using (var mmf = MemoryMappedFile.CreateFromFile(outputVfsPath, FileMode.Open, outputFilename))
            {
                IList<VFS.VirtualEntriesFilePartEntry> filesRead;

                // First pass: preload table with file data
                var firstPassBaseOffset = 0;
                using (var accessor = mmf.CreateViewAccessor(firstPassBaseOffset, headerLength))
                {
                    long position = 0;
                    accessor.Read(position, out long fileCapacity);
                    position += sizeof(long);

                    accessor.Read(position, out long headerTableSize);
                    position += sizeof(long);

                    // Get files count
                    filesRead = new List<VFS.VirtualEntriesFilePartEntry>((int)fileCapacity);


                    // Read all files
                    while(position != headerTableSize)
                    {
                        accessor.Read(position, out long fileOffset);
                        position += sizeof(long);

                        accessor.Read(position, out long fileBytes);
                        position += sizeof(long);

                        var fileName = new byte[256];
                        accessor.ReadArray(position, fileName, 0, sizeof(byte) * 256);
                        position += sizeof(byte) * 256;

                        filesRead.Add(new VFS.VirtualEntriesFilePartEntry
                        {
                            Offset = fileOffset,
                            Bytes = fileBytes,
                            FileName = fileName
                        });
                    }

                    foreach (var file in filesRead)
                    {
                        File.AppendAllText(logFilePath, $"VFS table entry: offset:'{file.Offset}'; size:'{file.Bytes}'; filename:'{file.FileName.FromByteArray()}'\r\n");
                        Console.WriteLine($"VFS table entry: offset:'{file.Offset}'; size:'{file.Bytes}'; filename:'{file.FileName.FromByteArray()}'");
                    }
                }
            }
#endif
        }
    }
}