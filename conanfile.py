from conan.tools.files import get
from conan import ConanFile

class keleidoscope(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("llvm-core/13.0.0")
        # self.requires("llvm-openmp/17.0.6")

#     [settings]
# arch=armv8
# build_type=Release
# compiler=apple-clang
# compiler.cppstd=gnu17
# compiler.libcxx=libc++
# compiler.version=15
# os=Macos
