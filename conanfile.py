from conans import ConanFile, CMake


class LibmuebConan(ConanFile):
    name = "libmueb"
    version = "4.0"
    description = "Schönherz Mátrix network library written in C++ using Qt"
    url = "https://git.sch.bme.hu/matrix-group/libmueb-qt"
    license = "LGPL-3.0-or-later"
    author = "Zsombor Bodnár (bodzsoaa@sch.bme.hu)"
    topics = ("C++", "Qt 6", "Schönherz Mátrix")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True], "fPIC": [True]}
    default_options = {"shared": True, "fPIC": True}
    requires = "qt/[^6.0.0]@bincrafters/stable"
    build_requires = "cmake/[^3.17.0]", "ninja/1.10.2"
    generators = "cmake"
    exports_sources = "CMakeLists.txt", "!CMakeLists.txt.user", "include/*", "src/*"

    def configure(self):
        if self.settings.compiler == "Visual Studio":
            del self.settings.compiler.runtime

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_WITH_CONAN"] = "ON"
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.release.libs = ["muebreceiver", "muebtransmitter"]
        self.cpp_info.debug.libs = ["muebreceiver_d", "muebtransmitter_d"]
