use edgevox_onnx;

fn main() {
    println!("Version : {}", edgevox_onnx::version());
    println!("Git SHA1: {}", edgevox_onnx::git_sha1());
    println!("Git date: {}", edgevox_onnx::git_date());
}
