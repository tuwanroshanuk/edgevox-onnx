/// Copyright (c)  2026  Xiaomi Corporation (authors: Fangjun Kuang)

namespace EdgevoxOnnx
{
    public struct OnlineSpeechDenoiserConfig
    {
        public OnlineSpeechDenoiserConfig()
        {
            Model = new OfflineSpeechDenoiserModelConfig();
        }

        public OfflineSpeechDenoiserModelConfig Model;
    }
}
