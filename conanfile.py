from conans import ConanFile, CMake


class LibmuebConan(ConanFile):
    name = "libmueb"
    version = "4.0"
    description = "Schönherz Mátrix network library written in C++ using Qt"
    url = "https://git.sch.bme.hu/kszk/schmatrix/libmueb"
    license = "LGPL-3.0-or-later"
    author = "Zsombor Bodnár (bodzsoaa@sch.bme.hu)"
    topics = ("C++", "Qt 5", "Schönherz Mátrix")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False],
               "websocket": [True, False], "tests": [True, False]}
    default_options = {"shared": True, "*:shared": True, "fPIC": True, "websocket": False, "tests": False}
    requires = "qt/[^5.15.2]"
    build_requires = "cmake/[^3.17.0]", "ninja/1.10.2"
    generators = "cmake_find_package"
    exports_sources = "CMakeLists.txt", "!CMakeLists.txt.user", "include/*", "src/*", "websocket/*", "tests/*"

    def configure(self):
        if self.settings.compiler == "Visual Studio":
            del self.settings.compiler.runtime

        if self.options.websocket:
            self.options["qt"].qtwebsockets = True

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self, "Ninja")
        if self.options.websocket:
            cmake.definitions["ENABLE_WEBSOCKET"] = "TRUE"

        if self.options.tests:
            cmake.definitions["ENABLE_TESTS"] = "TRUE"

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
        self.cpp_info.libs = ["muebreceiver", "muebtransmitter", "mueb"]
