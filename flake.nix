{
  description = "Project flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: 
  let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};

    # Cross compiler toolchain
    aarch64-elf-toolchain = pkgs.stdenv.mkDerivation {
      name = "aarch64-elf-toolchain";
      src = pkgs.fetchFromGitHub {
        owner = "radcolor";
        repo = "aarch64-elf";
        rev = "7a7fe924e86a4f1c6c14f2f59210649556af724c";
        hash = "sha256-Noelw80i+gsRZQNXz3s8Tqj+DnuD9y20SsWNvvhGHjI=";
      };
      
      installPhase = ''
        mkdir -p $out/bin
        cp $src/bin/* $out/bin/
      '';
         
    };
    
  in
  {
    devShells.${system} = {

      bits64 = pkgs.mkShell {

        packages = [ 
          pkgs.qemu
          pkgs.gdb
          aarch64-elf-toolchain
        ];

        shellHook = ''
          export CC="${aarch64-elf-toolchain.src}/bin/aarch64-elf-gcc"
          export AS="${aarch64-elf-toolchain.src}/bin/aarch64-elf-as"
          export LD="${aarch64-elf-toolchain.src}/bin/aarch64-elf-ld"
          export OBJCOPY="${aarch64-elf-toolchain.src}/bin/aarch64-elf-objcopy"
          export DBG="${pkgs.gdb}/bin/gdb"
        '';
      };

      bits32 = pkgs.mkShell {

        packages = [ 
          pkgs.qemu
          pkgs.gdb
          pkgs.gcc-arm-embedded
        ];

        shellHook = ''
          export PATH="$PATH:${pkgs.python3}/bin"
          export CC="${pkgs.gcc-arm-embedded}/bin/arm-none-eabi-gcc"
          export AS="${pkgs.gcc-arm-embedded}/bin/arm-none-eabi-as"
          export LD="${pkgs.gcc-arm-embedded}/bin/arm-none-eabi-ld"
          export OBJCOPY="${pkgs.gcc-arm-embedded}/bin/arm-none-eabi-objcopy"
          export DBG="${pkgs.gdb}/bin/gdb"
        '';
      };
    };
  };
}
