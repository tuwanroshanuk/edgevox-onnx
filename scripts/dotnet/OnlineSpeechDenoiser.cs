/// Copyright (c)  2026  Xiaomi Corporation (authors: Fangjun Kuang)

using System;
using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{
    public class OnlineSpeechDenoiser: IDisposable
    {
        public OnlineSpeechDenoiser(OnlineSpeechDenoiserConfig config)
        {
            IntPtr h = EdgevoxOnnxCreateOnlineSpeechDenoiser(ref config);
            _handle = new HandleRef(this, h);
        }

        public DenoisedAudio Run(float[] samples, int sampleRate)
        {
            IntPtr p = EdgevoxOnnxOnlineSpeechDenoiserRun(_handle.Handle, samples, samples.Length, sampleRate);
            return new DenoisedAudio(p);
        }

        public DenoisedAudio Flush()
        {
            IntPtr p = EdgevoxOnnxOnlineSpeechDenoiserFlush(_handle.Handle);
            return new DenoisedAudio(p);
        }

        public void Reset()
        {
            EdgevoxOnnxOnlineSpeechDenoiserReset(_handle.Handle);
        }

        public void Dispose()
        {
            Cleanup();
            System.GC.SuppressFinalize(this);
        }

        ~OnlineSpeechDenoiser()
        {
            Cleanup();
        }

        private void Cleanup()
        {
            EdgevoxOnnxDestroyOnlineSpeechDenoiser(_handle.Handle);
            _handle = new HandleRef(this, IntPtr.Zero);
        }

        private HandleRef _handle;

        public int SampleRate => EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate(_handle.Handle);

        public int FrameShiftInSamples =>
            EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples(_handle.Handle);

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxCreateOnlineSpeechDenoiser(ref OnlineSpeechDenoiserConfig config);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxDestroyOnlineSpeechDenoiser(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxOnlineSpeechDenoiserRun(IntPtr handle, float[] samples, int n, int sampleRate);

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxOnlineSpeechDenoiserFlush(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxOnlineSpeechDenoiserReset(IntPtr handle);
    }
}
