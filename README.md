# Remill Workshop

## Dev Container

This repository uses a [`devcontainer.json`](./.devcontainer/devcontainer.json) file to allow you to quickly get started:

- Install [Docker Desktop](https://www.docker.com/products/docker-desktop/) ([alternatives](https://code.visualstudio.com/remote/advancedcontainers/docker-options)) and [Visual Studio Code](https://code.visualstudio.com)
- Install the `Dev Containers` extension (you should be prompted when opening this folder)
- Open this repository
- Click the blue 'Reopen in Container' button when prompted

For more detailed steps, check out the [Dev Containers tutorial](https://code.visualstudio.com/docs/devcontainers/tutorial). The instructions after this assume you are running _inside_ the container.

## Building the examples

The container [`Dockerfile`](./.devcontainer/Dockerfile) contains all the tools and extensions necessary to develop LLVM-based tools in C++. To build the examples, use the following commands from VSCode's command palette (Meta+Shift+P):

- CMake: Configure
- CMake: Build
- clangd: Restart language server

## Activating the environment

Once you are inside the Dev Container, you can access the installed tools from `/cxx-common/install`. To add them to your `PATH`, run:

```bash
source llvm-env
```

To verify you should be able to run `clang-15 --version`:

```bash
clang version 15.0.4
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /cxx-common/install/bin
```

The `${workspaceFolder}/build` directory will also be added to your `PATH`, so you can easily access your tools from anywhere. To check if the examples built correctly you can run `hello`:

```sh
remill version: e72a100
```
