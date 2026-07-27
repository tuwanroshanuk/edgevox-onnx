/// Copyright (c)  2026  Xiaomi Corporation (authors: Fangjun Kuang)

using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{

    [StructLayout(LayoutKind.Sequential)]
    public struct OfflineFireRedAsrCtcModelConfig
    {
        public OfflineFireRedAsrCtcModelConfig()
        {
            Model = "";
        }
        [MarshalAs(UnmanagedType.LPStr)]
        public string Model;
    }
}
