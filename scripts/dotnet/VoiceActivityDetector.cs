/// Copyright (c)  2024  Xiaomi Corporation (authors: Fangjun Kuang)
using System;
using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{
    public class VoiceActivityDetector : IDisposable
    {
        public VoiceActivityDetector(VadModelConfig config, float bufferSizeInSeconds)
        {
            IntPtr h = EdgevoxOnnxCreateVoiceActivityDetector(ref config, bufferSizeInSeconds);
            _handle = new HandleRef(this, h);
        }

        public void AcceptWaveform(float[] samples)
        {
            EdgevoxOnnxVoiceActivityDetectorAcceptWaveform(_handle.Handle, samples, samples.Length);
        }

        public bool IsEmpty()
        {
            return EdgevoxOnnxVoiceActivityDetectorEmpty(_handle.Handle) == 1;
        }

        public bool IsSpeechDetected()
        {
            return EdgevoxOnnxVoiceActivityDetectorDetected(_handle.Handle) == 1;
        }

        public void Pop()
        {
            EdgevoxOnnxVoiceActivityDetectorPop(_handle.Handle);
        }

        public SpeechSegment Front()
        {
            IntPtr p = EdgevoxOnnxVoiceActivityDetectorFront(_handle.Handle);

            SpeechSegment segment = new SpeechSegment(p);

            EdgevoxOnnxDestroySpeechSegment(p);

            return segment;
        }

        public void Clear()
        {
            EdgevoxOnnxVoiceActivityDetectorClear(_handle.Handle);
        }

        public void Reset()
        {
            EdgevoxOnnxVoiceActivityDetectorReset(_handle.Handle);
        }

        public void Flush()
        {
            EdgevoxOnnxVoiceActivityDetectorFlush(_handle.Handle);
        }

        public void Dispose()
        {
            Cleanup();
            // Prevent the object from being placed on the
            // finalization queue
            System.GC.SuppressFinalize(this);
        }

        ~VoiceActivityDetector()
        {
            Cleanup();
        }

        private void Cleanup()
        {
            EdgevoxOnnxDestroyVoiceActivityDetector(_handle.Handle);

            // Don't permit the handle to be used again.
            _handle = new HandleRef(this, IntPtr.Zero);
        }

        private HandleRef _handle;

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxCreateVoiceActivityDetector(ref VadModelConfig config, float bufferSizeInSeconds);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxDestroyVoiceActivityDetector(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxVoiceActivityDetectorAcceptWaveform(IntPtr handle, float[] samples, int n);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxVoiceActivityDetectorEmpty(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxVoiceActivityDetectorDetected(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxVoiceActivityDetectorPop(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxVoiceActivityDetectorClear(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxVoiceActivityDetectorFront(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxDestroySpeechSegment(IntPtr segment);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxVoiceActivityDetectorReset(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxVoiceActivityDetectorFlush(IntPtr handle);
    }
}
