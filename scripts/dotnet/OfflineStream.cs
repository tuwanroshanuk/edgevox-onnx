/// Copyright (c)  2024.5 by 东风破

using System;
using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{

    public class OfflineStream : IDisposable
    {
        public OfflineStream(IntPtr p)
        {
            _handle = new HandleRef(this, p);
        }

        public void AcceptWaveform(int sampleRate, float[] samples)
        {
            AcceptWaveform(Handle, sampleRate, samples, samples.Length);
        }

        public void SetOption(string key, string value)
        {
            EdgevoxOnnxOfflineStreamSetOption(Handle, key, value);
        }

        public string GetOption(string key)
        {
            IntPtr p = EdgevoxOnnxOfflineStreamGetOption(Handle, key);
            return Marshal.PtrToStringAnsi(p) ?? "";
        }

        public bool HasOption(string key)
        {
            return EdgevoxOnnxOfflineStreamHasOption(Handle, key) == 1;
        }

        public OfflineRecognizerResult Result
        {
            get
            {
                IntPtr h = GetResult(_handle.Handle);
                OfflineRecognizerResult result = new OfflineRecognizerResult(h);
                DestroyResult(h);
                return result;
            }
        }

        ~OfflineStream()
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
            EdgevoxOnnxDestroyOfflineStream(Handle);

            // Don't permit the handle to be used again.
            _handle = new HandleRef(this, IntPtr.Zero);
        }

        private HandleRef _handle;
        public IntPtr Handle => _handle.Handle;

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxDestroyOfflineStream(IntPtr handle);

        [DllImport(Dll.Filename, EntryPoint = "EdgevoxOnnxAcceptWaveformOffline")]
        private static extern void AcceptWaveform(IntPtr handle, int sampleRate, float[] samples, int n);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxOfflineStreamSetOption(IntPtr handle, [MarshalAs(UnmanagedType.LPStr)] string key, [MarshalAs(UnmanagedType.LPStr)] string value);

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxOfflineStreamGetOption(IntPtr handle, [MarshalAs(UnmanagedType.LPStr)] string key);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxOfflineStreamHasOption(IntPtr handle, [MarshalAs(UnmanagedType.LPStr)] string key);

        [DllImport(Dll.Filename, EntryPoint = "EdgevoxOnnxGetOfflineStreamResult")]
        private static extern IntPtr GetResult(IntPtr handle);

        [DllImport(Dll.Filename, EntryPoint = "EdgevoxOnnxDestroyOfflineRecognizerResult")]
        private static extern void DestroyResult(IntPtr handle);
    }

}
