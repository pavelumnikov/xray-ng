using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MakeVirtualFileSystem.Utility
{
    internal interface ICommandLine
    {
        bool Has(string s);
        string Value(string s);
    }
}
