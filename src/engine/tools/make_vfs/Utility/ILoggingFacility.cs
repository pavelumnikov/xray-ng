namespace MakeVirtualFileSystem.Utility
{
    internal interface ILoggingFacility
    {
        void Log(string s);
        void LogFromThread(string s);
    }
}