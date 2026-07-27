/// Copyright (c)  2025  Xiaomi Corporation (authors: Fangjun Kuang)

using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{
    [StructLayout(LayoutKind.Sequential)]
    public struct OfflineSpeechDenoiserConfig
    {
        public OfflineSpeechDenoiserConfig()
        {
            Model = new OfflineSpeechDenoiserModelConfig();
        }
        public OfflineSpeechDenoiserModelConfig Model;
    }
}
