using System;

namespace MakeVirtualFileSystem.Utility
{
    internal interface ILoggingFacilityExecutor
    {
        void WaitForMessagesUntil(Func<bool> func);
    }
}
