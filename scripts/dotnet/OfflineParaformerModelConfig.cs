/// Copyright (c)  2024.5 by 东风破

using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{
    [StructLayout(LayoutKind.Sequential)]
    public struct OfflineParaformerModelConfig
    {
        public OfflineParaformerModelConfig()
        {
            Model = "";
        }
        [MarshalAs(UnmanagedType.LPStr)]
        public string Model;
    }

}