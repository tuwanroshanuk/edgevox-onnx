import platform

from setuptools import setup


def is_windows():
    return platform.system() == "Windows"


def get_binaries():
    if not is_windows():
        return None
    libs = [
        "onnxruntime.dll",
        "edgevox-onnx-c-api.dll",
        "edgevox-onnx-cxx-api.dll",
        "edgevox-onnx-c-api.lib",
        "edgevox-onnx-cxx-api.lib",
    ]
    prefix = "./edgevox_onnx/lib"
    return [f"{prefix}/{lib}" for lib in libs]


setup(
    name="edgevox-onnx-core",
    version="1.13.4",
    description="Core shared libraries for edgevox-onnx",
    packages=["edgevox_onnx"],
    include_package_data=True,
    data_files=[("Scripts", get_binaries())] if get_binaries() else None,
    author="The edgevox-onnx development team",
    url="https://github.com/k2-fsa/edgevox-onnx",
    author_email="dpovey@gmail.com",
    zip_safe=False,
    license="Apache-2.0",
    classifiers=[
        "Programming Language :: Python :: 3",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: POSIX :: Linux",
        "Operating System :: MacOS :: MacOS X",
        "Topic :: Scientific/Engineering :: Artificial Intelligence",
    ],
)
