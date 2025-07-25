from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout
from conan.tools.files import collect_libs, copy
from conan.tools.build import check_min_cppstd


class AtomtexSpeFileRecipe(ConanFile):
    name = "atomtex-spe-file"
    version = "0.1"
    description = "Parse *.spe files from ATOMTEX detector"

    package_type = "library"
    settings = "os", "compiler", "build_type", "arch"
    languages = "C++"

    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": True, "fPIC": True}

    build_tests = True

    def export_sources(self):
        self.build_tests = False
        copy(self, "src/*", self.recipe_folder, self.export_sources_folder)

    def requirements(self):
        if self.build_tests:
            self.requires("gtest/1.16.0")

    def validate(self):
        check_min_cppstd(self, 23)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure({"BUILD_TESTS": self.build_tests})
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "atomtex_spe_file")
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.libs = collect_libs(self)
