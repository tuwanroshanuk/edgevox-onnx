/// Copyright (c)  2025  Xiaomi Corporation (authors: Fangjun Kuang)

using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{

    [StructLayout(LayoutKind.Sequential)]
    public struct OfflineZipformerCtcModelConfig
    {
        public OfflineZipformerCtcModelConfig()
        {
            Model = "";
        }
        [MarshalAs(UnmanagedType.LPStr)]
        public string Model;
    }
}
