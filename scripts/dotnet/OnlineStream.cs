/// Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)
/// Copyright (c)  2023 by manyeyes
/// Copyright (c)  2024.5 by 东风破
using System;
using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{

    public class OnlineStream : IDisposable
    {
        public OnlineStream(IntPtr p)
        {
            _handle = new HandleRef(this, p);
        }

        public void AcceptWaveform(int sampleRate, float[] samples)
        {
            EdgevoxOnnxOnlineStreamAcceptWaveform(Handle, sampleRate, samples, samples.Length);
        }

        public void InputFinished()
        {
            EdgevoxOnnxOnlineStreamInputFinished(Handle);
        }

        public void SetOption(string key, string value)
        {
            EdgevoxOnnxOnlineStreamSetOption(Handle, key, value);
        }

        public string GetOption(string key)
        {
            IntPtr p = EdgevoxOnnxOnlineStreamGetOption(Handle, key);
            return Marshal.PtrToStringAnsi(p) ?? "";
        }

        public bool HasOption(string key)
        {
            return EdgevoxOnnxOnlineStreamHasOption(Handle, key) == 1;
        }

        ~OnlineStream()
        {
            Cleanup();
        }

        public void Dispose()
        {
            Cleanup();
            // Prevent the object from being placed on the
            // finalization queue
            System.GC.SuppressFinalize(this);
        }

        private void Cleanup()
        {
            EdgevoxOnnxDestroyOnlineStream(Handle);

            // Don't permit the handle to be used again.
            _handle = new HandleRef(this, IntPtr.Zero);
        }

        private HandleRef _handle;
        public IntPtr Handle => _handle.Handle;

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxDestroyOnlineStream(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxOnlineStreamAcceptWaveform(IntPtr handle, int sampleRate, float[] samples, int n);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxOnlineStreamInputFinished(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxOnlineStreamSetOption(IntPtr handle, [MarshalAs(UnmanagedType.LPStr)] string key, [MarshalAs(UnmanagedType.LPStr)] string value);

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxOnlineStreamGetOption(IntPtr handle, [MarshalAs(UnmanagedType.LPStr)] string key);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxOnlineStreamHasOption(IntPtr handle, [MarshalAs(UnmanagedType.LPStr)] string key);
    }

}
