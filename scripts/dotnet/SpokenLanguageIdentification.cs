/// Copyright (c)  2024.5 by 东风破
using System;
using System.Runtime.InteropServices;

namespace EdgevoxOnnx
{
    public class SpokenLanguageIdentification : IDisposable
{
    public SpokenLanguageIdentification(SpokenLanguageIdentificationConfig config)
    {
        IntPtr h = EdgevoxOnnxCreateSpokenLanguageIdentification(ref config);
        _handle = new HandleRef(this, h);
    }

    public OfflineStream CreateStream()
    {
        IntPtr p = EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream(_handle.Handle);
        return new OfflineStream(p);
    }

    public SpokenLanguageIdentificationResult Compute(OfflineStream stream)
    {
        IntPtr h = EdgevoxOnnxSpokenLanguageIdentificationCompute(_handle.Handle, stream.Handle);
        SpokenLanguageIdentificationResult result = new SpokenLanguageIdentificationResult(h);
        EdgevoxOnnxDestroySpokenLanguageIdentificationResult(h);
        return result;
    }

    public void Dispose()
    {
        Cleanup();
        // Prevent the object from being placed on the
        // finalization queue
        System.GC.SuppressFinalize(this);
    }

    ~SpokenLanguageIdentification()
    {
        Cleanup();
    }

    private void Cleanup()
    {
        EdgevoxOnnxDestroySpokenLanguageIdentification(_handle.Handle);

        // Don't permit the handle to be used again.
        _handle = new HandleRef(this, IntPtr.Zero);
    }

    private HandleRef _handle;

    [DllImport(Dll.Filename)]
    private static extern IntPtr EdgevoxOnnxCreateSpokenLanguageIdentification(ref SpokenLanguageIdentificationConfig config);

    [DllImport(Dll.Filename)]
    private static extern void EdgevoxOnnxDestroySpokenLanguageIdentification(IntPtr handle);

    [DllImport(Dll.Filename)]
    private static extern IntPtr EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream(IntPtr handle);

    [DllImport(Dll.Filename)]
    private static extern IntPtr EdgevoxOnnxSpokenLanguageIdentificationCompute(IntPtr handle, IntPtr stream);

    [DllImport(Dll.Filename)]
    private static extern void EdgevoxOnnxDestroySpokenLanguageIdentificationResult(IntPtr handle);
}
}
