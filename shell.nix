let
  pkgsTarball = fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/tarball/11cb3517b3af6af300dd6c055aeda73c9bf52c48";
    sha256 = "1915r28xc4znrh2vf4rrjnxldw2imysz819gzhk9qlrkqanmfsxd";
  };
  pkgs = import pkgsTarball {
    config = {}; overlays = [];
  };
in

pkgs.mkShellNoCC {
  packages = with pkgs; [
    conan
    cmake
    gnumake
    llvmPackages_20.clang
  ];
  shellHook = ''
    export CONAN_HOME="$PWD/.conan2"
  '';
}
