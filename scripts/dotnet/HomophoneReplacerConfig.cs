/// Copyright (c)  2025  Xiaomi Corporation (authors: Fangjun Kuang)

using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{
    [StructLayout(LayoutKind.Sequential)]
    public struct HomophoneReplacerConfig
    {
        public HomophoneReplacerConfig()
        {
          DictDir = "";
          Lexicon = "";
          RuleFsts = "";
        }

        [MarshalAs(UnmanagedType.LPStr)]
        public string DictDir;

        [MarshalAs(UnmanagedType.LPStr)]
        public string Lexicon;

        [MarshalAs(UnmanagedType.LPStr)]
        public string RuleFsts;
    }
}
