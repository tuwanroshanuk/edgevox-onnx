/// Copyright (c)  2024  Xiaomi Corporation (authors: Fangjun Kuang)

using System;
using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{
    public class CircularBuffer : IDisposable
    {
        public CircularBuffer(int capacity)
        {
            IntPtr h = EdgevoxOnnxCreateCircularBuffer(capacity);
            _handle = new HandleRef(this, h);
        }

        public void Push(float[] data)
        {
            EdgevoxOnnxCircularBufferPush(_handle.Handle, data, data.Length);
        }

        public float[] Get(int startIndex, int n)
        {
            IntPtr p = EdgevoxOnnxCircularBufferGet(_handle.Handle, startIndex, n);

            float[] ans = new float[n];
            Marshal.Copy(p, ans, 0, n);

            EdgevoxOnnxCircularBufferFree(p);

            return ans;
        }

        public void Pop(int n)
        {
            EdgevoxOnnxCircularBufferPop(_handle.Handle, n);
        }

        public int Size
        {
          get
          {
              return EdgevoxOnnxCircularBufferSize(_handle.Handle);
          }
        }

        public int Head
        {
          get
          {
              return EdgevoxOnnxCircularBufferHead(_handle.Handle);
          }
        }

        public void Reset()
        {
            EdgevoxOnnxCircularBufferReset(_handle.Handle);
        }

        public void Dispose()
        {
            Cleanup();
            // Prevent the object from being placed on the
            // finalization queue
            System.GC.SuppressFinalize(this);
        }

        ~CircularBuffer()
        {
            Cleanup();
        }

        private void Cleanup()
        {
            EdgevoxOnnxDestroyCircularBuffer(_handle.Handle);

            // Don't permit the handle to be used again.
            _handle = new HandleRef(this, IntPtr.Zero);
        }

        private HandleRef _handle;

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxCreateCircularBuffer(int capacity);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxDestroyCircularBuffer(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxCircularBufferPush(IntPtr handle, float[] p, int n);

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxCircularBufferGet(IntPtr handle, int startIndex, int n);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxCircularBufferFree(IntPtr p);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxCircularBufferPop(IntPtr handle, int n);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxCircularBufferSize(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxCircularBufferHead(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxCircularBufferReset(IntPtr handle);
    }
}
