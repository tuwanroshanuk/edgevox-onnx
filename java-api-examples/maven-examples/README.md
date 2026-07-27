# edgevox-onnx Maven Example

This directory demonstrates how to use [edgevox-onnx](https://github.com/k2-fsa/edgevox-onnx) via Maven.

## Prerequisites

- JDK 8 or above
- Maven 3.x

## Dependencies

The project uses [JitPack](https://jitpack.io/) to fetch edgevox-onnx artifacts.
There are two ways to declare the dependencies.

### Approach 1: Simple (single dependency)

This is the easiest way — one dependency pulls in everything:

```xml
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>

<dependency>
    <groupId>com.github.k2-fsa</groupId>
    <artifactId>edgevox-onnx</artifactId>
    <version>refactor-jar-SNAPSHOT</version>
</dependency>
```

### Approach 2: Multi-module (recommended)

This is the recommended way for production use. It splits the JVM API and native libs
so you only ship the platform you need, keeping the final jar lightweight:

```xml
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>

<!-- 1. JVM core API -->
<dependency>
    <groupId>com.github.k2-fsa.edgevox-onnx</groupId>
    <artifactId>edgevox-onnx-jvm</artifactId>
    <version>refactor-jar-SNAPSHOT</version>
</dependency>

<!-- 2. Platform native lib — pick ONE for your target platform -->
<dependency>
    <groupId>com.github.k2-fsa.edgevox-onnx</groupId>
    <artifactId>edgevox-onnx-native-lib-osx-aarch64</artifactId>
    <version>refactor-jar-SNAPSHOT</version>
</dependency>
```

Available native lib artifacts for each platform:

| Platform | Artifact |
|---|---|
| macOS ARM64 | `edgevox-onnx-native-lib-osx-aarch64` |
| macOS x64 | `edgevox-onnx-native-lib-osx-x64` |
| Linux x64 | `edgevox-onnx-native-lib-linux-x64` |
| Linux ARM64 | `edgevox-onnx-native-lib-linux-aarch64` |
| Windows x64 | `edgevox-onnx-native-lib-win-x64` |

> **Note:** The `pom.xml` in this directory uses Approach 2 by default. Approach 1 is
> included as a comment for reference.

## Build

```bash
cd java-api-examples/maven-examples
mvn package -q
```

## Run

```bash
java -jar target/edgevox-onnx-maven-example-1.0-SNAPSHOT.jar
```

Expected output:

```
edgevox-onnx version: x.y.z
edgevox-onnx gitSha1: ...
edgevox-onnx gitDate: ...
```

## Appendix: Approach 1 vs Approach 2 comparison (advanced)

The two approaches produce **very different** fat jars:

| | Approach 1 (simple) | Approach 2 (multi-module) |
|---|---|---|
| Jar size (compressed) | **95 MB** | **9 MB** |
| Uncompressed size | **286 MB** | **33 MB** |
| Total files | 295 | 253 |
| JVM class files | 236 | 236 |
| Native libs included | ALL platforms | macOS ARM64 only |

### What is inside each jar?

**Approach 1** (`com.github.k2-fsa:edgevox-onnx`) bundles native libs for every platform:

```
jni/arm64-v8a/libonnxruntime.so              21 MB   (Android ARM64)
jni/arm64-v8a/libedgevox-onnx-c-api.so        4.4 MB
jni/arm64-v8a/libedgevox-onnx-cxx-api.so      440 KB
jni/arm64-v8a/libedgevox-onnx-jni.so          4.7 MB
jni/armeabi-v7a/libonnxruntime.so            15 MB   (Android ARMv7)
jni/armeabi-v7a/libedgevox-onnx-c-api.so      3.2 MB
jni/armeabi-v7a/libedgevox-onnx-cxx-api.so    282 KB
jni/armeabi-v7a/libedgevox-onnx-jni.so        3.4 MB
jni/x86/libonnxruntime.so                    26 MB   (Android x86)
jni/x86/libedgevox-onnx-c-api.so              5.0 MB
jni/x86/libedgevox-onnx-cxx-api.so            392 KB
jni/x86/libedgevox-onnx-jni.so                5.3 MB
jni/x86_64/libonnxruntime.so                 25 MB   (Android x86_64)
jni/x86_64/libedgevox-onnx-c-api.so           4.9 MB
jni/x86_64/libedgevox-onnx-cxx-api.so         437 KB
jni/x86_64/libedgevox-onnx-jni.so             5.1 MB
edgevox-onnx/native/linux-aarch64/libonnxruntime.so       34 MB   (Linux ARM64)
edgevox-onnx/native/linux-aarch64/libedgevox-onnx-jni.so   4.7 MB
edgevox-onnx/native/linux-x64/libonnxruntime.so           26 MB   (Linux x64)
edgevox-onnx/native/linux-x64/libedgevox-onnx-jni.so       5.1 MB
edgevox-onnx/native/osx-aarch64/libonnxruntime.1.27.0.dylib  28 MB   (macOS ARM64)
edgevox-onnx/native/osx-aarch64/libedgevox-onnx-jni.dylib     4.2 MB
edgevox-onnx/native/osx-x64/libonnxruntime.1.27.0.dylib      32 MB   (macOS x64)
edgevox-onnx/native/osx-x64/libedgevox-onnx-jni.dylib         4.4 MB
edgevox-onnx/native/win-x64/onnxruntime.dll             17 MB   (Windows x64)
edgevox-onnx/native/win-x64/edgevox-onnx-jni.dll         4.6 MB
```

**Approach 2** (`edgevox-onnx-jvm` + `edgevox-onnx-native-lib-osx-aarch64`) only includes
the macOS ARM64 native libs:

```
edgevox-onnx/native/osx-aarch64/libonnxruntime.1.27.0.dylib  28 MB   (macOS ARM64)
edgevox-onnx/native/osx-aarch64/libedgevox-onnx-jni.dylib     4.2 MB
```

### Why Approach 2 is recommended

Approach 2 is **10x smaller** (9 MB vs 95 MB) because it only ships the native libs for
your target platform. This matters for:

- **Download size**: users only download what they need
- **Startup time**: the JVM loads fewer native libraries
- **Disk space**: especially important for CI/CD and container images

### Verify it yourself

```bash
# Build and check the jar
mvn package -q
ls -lh target/edgevox-onnx-maven-example-1.0-SNAPSHOT.jar

# List native libs in the jar
unzip -l target/edgevox-onnx-maven-example-1.0-SNAPSHOT.jar | grep -E "\.so$|\.dylib$|\.dll$"
```
