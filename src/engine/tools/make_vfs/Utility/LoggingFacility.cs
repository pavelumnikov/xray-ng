using System;
using System.Collections.Concurrent;
using System.IO;
using System.Threading;

namespace MakeVirtualFileSystem.Utility
{
    internal class LoggingFacility : ILoggingFacility, ILoggingFacilityExecutor
    {
        private readonly string _path;
        private readonly ConcurrentQueue<string> _messages;

        public LoggingFacility(string path)
        {
            _path = path;
            _messages = new ConcurrentQueue<string>();
            File.WriteAllText(_path, "--- Starting logging session ---\r\n");
        }

        public void Log(string s)
        {
            var time = DateTime.Now;
            var formatted = $"[THREAD: MAIN][{time.ToLocalTime().ToShortDateString()}] {s}";
            DispatchLogMessage(formatted);
        }

        public void LogFromThread(string s)
        {
            var threadId = Thread.CurrentThread.ManagedThreadId;
            var time = DateTime.Now;
            var formatted = $"[THREAD:{threadId}][{time.ToLocalTime().ToShortDateString()}] {s}";
            _messages.Enqueue(formatted);
        }

        public void WaitForMessagesUntil(Func<bool> func)
        {
            while(func() || !_messages.IsEmpty)
            {
                var result = _messages.TryDequeue(out string output);
                if (result == false)
                {
                    Thread.Sleep(1);
                    continue;
                }

                DispatchLogMessage(output);
            }
        }

        private void DispatchLogMessage(string s)
        {
            File.AppendAllText(_path, s);
            Console.WriteLine(s);
        }
    }
}
