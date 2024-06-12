#!/bin/bash
if [ ! -d "klee" ]; then
    echo "[PROGRESS] Cloning KLEE and installing dependencies..."
    git clone https://github.com/klee/klee --depth=1
    apt update && apt install -y libz-dev libsqlite3-dev sqlite3
fi

echo "[PROGRESS] Configuring KLEE..."
cmake -B klee/build -S klee -DENABLE_TCMALLOC=OFF -DENABLE_UNIT_TESTS=OFF -DENABLE_SYSTEM_TESTS=OFF -DENABLE_POSIX_RUNTIME=OFF -G Ninja -DCMAKE_INSTALL_PREFIX=/cxx-common/install
if [ $? -ne 0 ]; then
  echo "[ERROR] Failed to configure KLEE (reach out to your instructor for help)"
  exit 1
fi

echo "[PROGRESS] Building KLEE..."
cmake --build klee/build
if [ $? -ne 0 ]; then
  echo "[ERROR] Failed to build KLEE (reach out to your instructor for help)"
  exit 1
fi

echo "[PROGRESS] Installing KLEE..."
cmake --install klee/build
if [ $? -ne 0 ]; then
  echo "[ERROR] Failed to install KLEE (reach out to your instructor for help)"
  exit 1
fi

echo ""
echo "[SUCCESS] KLEE was successfully installed at $(which klee)"
echo ""

klee --version
