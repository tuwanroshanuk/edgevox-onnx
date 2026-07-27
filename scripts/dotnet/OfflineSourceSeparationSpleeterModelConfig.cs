/// Copyright (c)  2026  Xiaomi Corporation (authors: Fangjun Kuang)

using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{
    [StructLayout(LayoutKind.Sequential)]
    public struct OfflineSourceSeparationSpleeterModelConfig
    {
        public OfflineSourceSeparationSpleeterModelConfig()
        {
            Vocals = "";
            Accompaniment = "";
        }

        [MarshalAs(UnmanagedType.LPStr)]
        public string Vocals;

        [MarshalAs(UnmanagedType.LPStr)]
        public string Accompaniment;
    }
}
