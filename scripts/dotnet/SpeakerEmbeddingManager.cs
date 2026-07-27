/// Copyright (c)  2024.5 by 东风破
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace EdgevoxOnnx
{
    public class SpeakerEmbeddingManager : IDisposable
    {
        public SpeakerEmbeddingManager(int dim)
        {
            IntPtr h = EdgevoxOnnxCreateSpeakerEmbeddingManager(dim);
            _handle = new HandleRef(this, h);
            this._dim = dim;
        }

        public bool Add(string name, float[] v)
        {
            byte[] utf8Name = Encoding.UTF8.GetBytes(name);
            byte[] utf8NameWithNull = new byte[utf8Name.Length + 1]; // +1 for null terminator
            Array.Copy(utf8Name, utf8NameWithNull, utf8Name.Length);
            utf8NameWithNull[utf8Name.Length] = 0; // Null terminator
            return EdgevoxOnnxSpeakerEmbeddingManagerAdd(_handle.Handle, utf8NameWithNull, v) == 1;
        }

        public bool Add(string name, ICollection<float[]> v_list)
        {
            int n = v_list.Count;
            float[] v = new float[n * _dim];
            int i = 0;
            foreach (var item in v_list)
            {
                item.CopyTo(v, i);
                i += _dim;
            }

            byte[] utf8Name = Encoding.UTF8.GetBytes(name);
            byte[] utf8NameWithNull = new byte[utf8Name.Length + 1]; // +1 for null terminator
            Array.Copy(utf8Name, utf8NameWithNull, utf8Name.Length);
            utf8NameWithNull[utf8Name.Length] = 0; // Null terminator
            return EdgevoxOnnxSpeakerEmbeddingManagerAddListFlattened(_handle.Handle, utf8NameWithNull, v, n) == 1;
        }

        public bool Remove(string name)
        {
            byte[] utf8Name = Encoding.UTF8.GetBytes(name);
            byte[] utf8NameWithNull = new byte[utf8Name.Length + 1]; // +1 for null terminator
            Array.Copy(utf8Name, utf8NameWithNull, utf8Name.Length);
            utf8NameWithNull[utf8Name.Length] = 0; // Null terminator
            return EdgevoxOnnxSpeakerEmbeddingManagerRemove(_handle.Handle, utf8NameWithNull) == 1;
        }

        public string Search(float[] v, float threshold)
        {
            IntPtr p = EdgevoxOnnxSpeakerEmbeddingManagerSearch(_handle.Handle, v, threshold);

            string s = "";
            int length = 0;

            unsafe
            {
                byte* b = (byte*)p;
                if (b != null)
                {
                    while (*b != 0)
                    {
                        ++b;
                        length += 1;
                    }
                }
            }

            if (length > 0)
            {
                byte[] stringBuffer = new byte[length];
                Marshal.Copy(p, stringBuffer, 0, length);
                s = Encoding.UTF8.GetString(stringBuffer);
            }

            EdgevoxOnnxSpeakerEmbeddingManagerFreeSearch(p);

            return s;
        }

        public bool Verify(string name, float[] v, float threshold)
        {
            byte[] utf8Name = Encoding.UTF8.GetBytes(name);
            byte[] utf8NameWithNull = new byte[utf8Name.Length + 1]; // +1 for null terminator
            Array.Copy(utf8Name, utf8NameWithNull, utf8Name.Length);
            utf8NameWithNull[utf8Name.Length] = 0; // Null terminator
            return EdgevoxOnnxSpeakerEmbeddingManagerVerify(_handle.Handle, utf8NameWithNull, v, threshold) == 1;
        }

        public bool Contains(string name)
        {
            byte[] utf8Name = Encoding.UTF8.GetBytes(name);
            byte[] utf8NameWithNull = new byte[utf8Name.Length + 1]; // +1 for null terminator
            Array.Copy(utf8Name, utf8NameWithNull, utf8Name.Length);
            utf8NameWithNull[utf8Name.Length] = 0; // Null terminator
            return EdgevoxOnnxSpeakerEmbeddingManagerContains(_handle.Handle, utf8NameWithNull) == 1;
        }

        public string[] GetAllSpeakers()
        {
            if (NumSpeakers == 0)
            {
                return new string[] { };
            }

            IntPtr names = EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakers(_handle.Handle);

            string[] ans = new string[NumSpeakers];

            unsafe
            {
                byte** p = (byte**)names;
                for (int i = 0; i != NumSpeakers; i++)
                {
                    int length = 0;
                    byte* s = p[i];
                    while (*s != 0)
                    {
                        ++s;
                        length += 1;
                    }
                    byte[] stringBuffer = new byte[length];
                    Marshal.Copy((IntPtr)p[i], stringBuffer, 0, length);
                    ans[i] = Encoding.UTF8.GetString(stringBuffer);
                }
            }

            EdgevoxOnnxSpeakerEmbeddingManagerFreeAllSpeakers(names);

            return ans;
        }

        public void Dispose()
        {
            Cleanup();
            // Prevent the object from being placed on the
            // finalization queue
            System.GC.SuppressFinalize(this);
        }

        ~SpeakerEmbeddingManager()
        {
            Cleanup();
        }

        private void Cleanup()
        {
            EdgevoxOnnxDestroySpeakerEmbeddingManager(_handle.Handle);

            // Don't permit the handle to be used again.
            _handle = new HandleRef(this, IntPtr.Zero);
        }

        public int NumSpeakers
        {
            get
            {
                return EdgevoxOnnxSpeakerEmbeddingManagerNumSpeakers(_handle.Handle);
            }
        }

        private HandleRef _handle;
        private int _dim;


        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxCreateSpeakerEmbeddingManager(int dim);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxDestroySpeakerEmbeddingManager(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxSpeakerEmbeddingManagerAdd(IntPtr handle, [MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I1)] byte[] utf8Name, float[] v);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxSpeakerEmbeddingManagerAddListFlattened(IntPtr handle, [MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I1)] byte[] utf8Name, float[] v, int n);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxSpeakerEmbeddingManagerRemove(IntPtr handle, [MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I1)] byte[] utf8Name);

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxSpeakerEmbeddingManagerSearch(IntPtr handle, float[] v, float threshold);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxSpeakerEmbeddingManagerFreeSearch(IntPtr p);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxSpeakerEmbeddingManagerVerify(IntPtr handle, [MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I1)] byte[] utf8Name, float[] v, float threshold);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxSpeakerEmbeddingManagerContains(IntPtr handle, [MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I1)] byte[] utf8Name);

        [DllImport(Dll.Filename)]
        private static extern int EdgevoxOnnxSpeakerEmbeddingManagerNumSpeakers(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakers(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern void EdgevoxOnnxSpeakerEmbeddingManagerFreeAllSpeakers(IntPtr names);
    }
}
