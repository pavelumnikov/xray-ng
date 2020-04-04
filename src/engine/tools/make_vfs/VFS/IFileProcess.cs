namespace MakeVirtualFileSystem.VFS
{
    internal interface IFileProcess
    {
        void Begin(string basePath, string packFile);
        bool IsDone();
    }
}
