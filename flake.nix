{
  description = "Dev shells to build this project";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-25.05";
  };

  outputs = { self, nixpkgs }:
    let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
      mingw64Pkgs = pkgs.pkgsCross.mingwW64.buildPackages;
      commonBuildInputs = with pkgs; [
        conan
        cmake
        gnumake
      ];
    in {
      devShells.x86_64-linux = {

        # shell for native build
        native = pkgs.mkShellNoCC {
          buildInputs = [
            commonBuildInputs
            pkgs.llvmPackages_20.clang
          ];
          shellHook = ''
            export CONAN_HOME="$PWD/.conan2"
          '';
        };
        default = self.devShells.x86_64-linux.native;

        # shell for Windows cross-compilation
        mingw64 = pkgs.mkShellNoCC {
          buildInputs = [
            commonBuildInputs
            mingw64Pkgs.gcc
            mingw64Pkgs.binutils
          ];
          shellHook = ''
            export CONAN_HOME="$PWD/.conan2"
          '';
        };

      };
    };
}
