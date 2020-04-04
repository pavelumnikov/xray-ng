using System;

namespace MakeVirtualFileSystem.VFS
{
    internal class UnpackFileProcess : IFileProcess
    {
        private readonly Utility.ILoggingFacility _loggingFacility;

        public UnpackFileProcess(Utility.ILoggingFacility loggingFacility)
        {
            _loggingFacility = loggingFacility;
        }

        public void Begin(string basePath, string packFile)
        {
            throw new NotImplementedException();
        }

        public bool IsDone()
        {
            throw new NotImplementedException();
        }
    }
}
