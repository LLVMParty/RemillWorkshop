FROM ghcr.io/mrexodia/cxx-common-cmake:20240808_b94d6786

RUN apt update \
 && apt install -y \
        git \
        clangd \
        clang-format \
        cmake \
        ninja-build \
        gdb \
        bsdmainutils \
        binutils \
 && apt autoremove -y \
 && rm -rf /var/lib/apt/lists/*

LABEL org.opencontainers.image.source=https://github.com/LLVMParty/RemillWorkshop
