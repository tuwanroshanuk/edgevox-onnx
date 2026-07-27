/// Copyright (c)  2024.5 by 东风破
using System;
using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{
    public class SpeakerEmbeddingExtractor : IDisposable
    {
        public SpeakerEmbeddingExtractor(SpeakerEmbeddingExtractorConfig config)
        {
            IntPtr h = EdgevoxOnnxCreateSpeakerEmbeddingExtractor(ref config);
            _handle = new HandleRef(this, h);
        }

        public OnlineStream CreateStream()
        {
            IntPtr p = EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream(_handle.Handle);
            return new OnlineStream(p);
        }

        public bool IsReady(OnlineStream stream)
        {
            return EdgevoxOnnxSpeakerEmbeddingExtractorIsReady(_handle.Handle, stream.Handle) != 0;
        }

        public float[] Compute(OnlineStream stream)
        {
            IntPtr p = EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding(_handle.Handle, stream.Handle);

            int dim = Dim;
            float[] ans = new float[dim];
            Marshal.Copy(p, ans, 0, dim);

            EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbedding(p);

            return ans;
        }

        public int Dim
        {
            get
            {
                return EdgevoxOnnxSpeakerEmbeddingExtractorDim(_handle.Handle);
            }
        }

        public void Dispose()
        {
            Cleanup();
            // Prevent the object from being placed on the
            // finalization queue
            System.GC.SuppressFinalize(this);
        }

        ~SpeakerEmbeddingExtractor()
        {
            Cleanup();
        }

        private void Cleanup()
        {
            EdgevoxOnnxDestroySpeakerEmbeddingExtractor(_handle.Handle);

            // Don't permit the handle to be used again.
            _handle = new HandleRef(this, IntPtr.Zero);
        }

        private HandleRef _handle;

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxCreateSpeakerEmbeddingExtractor(ref SpeakerEmbeddingExtractorConfig config);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxDestroySpeakerEmbeddingExtractor(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxSpeakerEmbeddingExtractorDim(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxSpeakerEmbeddingExtractorIsReady(IntPtr handle, IntPtr stream);

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding(IntPtr handle, IntPtr stream);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbedding(IntPtr p);
    }

}
