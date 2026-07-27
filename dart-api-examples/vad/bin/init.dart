// Copyright (c)  2024  Xiaomi Corporation
import 'dart:io';
import 'dart:isolate';
import 'package:path/path.dart' as p;
import 'package:edgevox_onnx/edgevox_onnx.dart' as edgevox_onnx;

Future<void> initEdgevoxOnnx() async {
  String platform = '';

  if (Platform.isMacOS) {
    platform = 'macos';
  } else if (Platform.isLinux) {
    platform = 'linux';
  } else if (Platform.isWindows) {
    platform = 'windows';
  } else {
    throw UnsupportedError('Unknown platform: ${Platform.operatingSystem}');
  }

  var uri = await Isolate.resolvePackageUri(
      Uri.parse('package:edgevox_onnx_$platform/any_path_is_ok_here.dart'));

  if (uri == null) {
    print('File not found');
    exit(1);
  }

  var libPath = p.join(p.dirname(p.fromUri(uri)), '..', platform);
  if (platform == 'linux') {
    final arch = Platform.version.contains('arm64') ||
            Platform.version.contains('aarch64')
        ? 'aarch64'
        : 'x64';
    libPath = p.join(p.dirname(p.fromUri(uri)), '..', platform, arch);
  }

  edgevox_onnx.initBindings(libPath);
}
