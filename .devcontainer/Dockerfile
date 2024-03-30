FROM ghcr.io/mrexodia/cxx-common-cmake:8f93b1cf

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
        libstdc++-11-dev \
        libxml2-dev \
        ncurses-dev \
 && apt remove -y gcc gcc-11 \
 && apt autoremove -y \
 && rm -rf /var/lib/apt/lists/*

LABEL org.opencontainers.image.source=https://github.com/LLVMParty/RemillWorkshop
