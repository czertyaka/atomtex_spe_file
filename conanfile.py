from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conan.tools.files import collect_libs, copy
from conan.tools.build import check_min_cppstd, cross_building


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
    build_benchmark = False

    def export_sources(self):
        self.build_tests = False
        self.build_benchmark = False
        copy(self, "src/*", self.recipe_folder, self.export_sources_folder)

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("boost/[>=1.85]")
        if not cross_building(self):
            if self.build_tests:
                self.requires("gtest/1.16.0")
            if self.build_benchmark:
                self.requires("benchmark/1.9.4")

    def validate(self):
        check_min_cppstd(self, 23)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def layout(self):
        folder_vars = [
            "settings.build_type",
            "settings.os",
            "settings.compiler",
            "options.shared",
        ]
        self.folders.build_folder_vars = folder_vars
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        vars = dict()
        if not cross_building(self):
            vars["BUILD_TESTS"] = self.build_tests
            vars["BUILD_BENCHMARK"] = self.build_benchmark
        cmake.configure(variables=vars)
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "atomtex_spe_file")
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.libs = collect_libs(self)
