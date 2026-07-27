// Copyright (c)  2025  Xiaomi Corporation
using EdgevoxOnnx;

class VersionTestDemo
{
  static void Main(string[] args)
  {
    var version = VersionInfo.Version;
    var gitSha1 = VersionInfo.GitSha1;
    var gitDate = VersionInfo.GitDate;

    Console.WriteLine("edgevox-onnx version: {0}", version);
    Console.WriteLine("edgevox-onnx gitSha1: {0}", gitSha1);
    Console.WriteLine("edgevox-onnx gitDate: {0}", gitDate);
  }
}
