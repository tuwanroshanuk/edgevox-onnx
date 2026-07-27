func run() {
  let version = getEdgevoxOnnxVersion()
  let gitSha1 = getEdgevoxOnnxGitSha1()
  let gitDate = getEdgevoxOnnxGitDate()
  print("edgevox-onnx version: \(version)")
  print("edgevox-onnx gitSha1: \(gitSha1)")
  print("edgevox-onnx gitDate: \(gitDate)")
}

@main
struct App {
  static func main() {
    run()
  }
}
