using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace UserConsole.Code
{
    public class API
    {
        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int RechargeCard(string strCardNo, StringBuilder sbRetMsg);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CheckCard(string strCard);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int QueryCardLog(StringBuilder LogContent);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Login(string strAccountName, string strAccountPass);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Register(string strAccountName, string strAccountPass);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetConfig(string strConfigName, string strConfigValue);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SaveConfig();

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetHour();

    }
}
