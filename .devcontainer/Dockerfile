FROM ghcr.io/mrexodia/cxx-common-cmake:8f93b1cf

RUN apt update \
 && apt install -y \
        git \
        clang \
        clangd \
        clang-format \
        cmake \
        ninja-build \
        gdb \
 && apt remove -y gcc gcc-11
