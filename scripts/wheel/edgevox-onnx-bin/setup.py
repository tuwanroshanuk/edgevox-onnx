import glob
import platform

from setuptools import setup


def is_windows():
    return platform.system() == "Windows"


bin_files = glob.glob("bin/*")
print("bin_files", bin_files)

setup(
    name="edgevox-onnx-bin",
    version="1.13.4",
    description="Binary executables for edgevox-onnx",
    author="The edgevox-onnx development team",
    url="https://github.com/k2-fsa/edgevox-onnx",
    author_email="dpovey@gmail.com",
    zip_safe=False,
    license="Apache 2.0",
    packages=[],
    data_files=[("Scripts", bin_files) if is_windows() else ("bin", bin_files)],
    install_requires=[
        "edgevox-onnx-core==1.13.4",
    ],
    classifiers=[
        "Programming Language :: Python :: 3",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: POSIX :: Linux",
        "Operating System :: MacOS :: MacOS X",
        "Topic :: Scientific/Engineering :: Artificial Intelligence",
    ],
)
