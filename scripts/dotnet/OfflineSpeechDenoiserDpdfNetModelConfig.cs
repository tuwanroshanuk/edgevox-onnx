/// Copyright (c)  2026  Xiaomi Corporation

using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{
    [StructLayout(LayoutKind.Sequential)]
    public struct OfflineSpeechDenoiserDpdfNetModelConfig
    {
        public OfflineSpeechDenoiserDpdfNetModelConfig()
        {
            Model = "";
        }

        [MarshalAs(UnmanagedType.LPStr)]
        public string Model;
    }
}
