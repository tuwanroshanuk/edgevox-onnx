/// Copyright (c)  2024  Xiaomi Corporation (authors: Fangjun Kuang)

using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{
    [StructLayout(LayoutKind.Sequential)]
    public struct OfflinePunctuationConfig
    {
        public OfflinePunctuationConfig()
        {
            Model = new OfflinePunctuationModelConfig();
        }
        public OfflinePunctuationModelConfig Model;
    }
}

