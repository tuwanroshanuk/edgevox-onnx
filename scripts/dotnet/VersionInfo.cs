/// Copyright (c)  2025  Xiaomi Corporation (authors: Fangjun Kuang)
using System;
using System.Runtime.InteropServices;
using System.Text;


namespace EdgevoxOnnx
{
    public class VersionInfo
    {
        public static String Version
        {
          get
          {
            IntPtr p = EdgevoxOnnxGetVersionStr();

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

            return s;
          }
        }

        public static String GitSha1
        {
          get
          {
            IntPtr p = EdgevoxOnnxGetGitSha1();

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

            return s;
          }
        }

        public static String GitDate
        {
          get
          {
            IntPtr p = EdgevoxOnnxGetGitDate();

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

            return s;
          }
        }


        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxGetVersionStr();

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxGetGitSha1();

        [DllImport(Dll.Filename)]
        private static extern IntPtr EdgevoxOnnxGetGitDate();
    }
}
