# Remill Workshop

## Dev Container

This repository uses a [`devcontainer.json`](./.devcontainer/devcontainer.json) file to allow you to quickly get started:

- Install/Update [Docker Desktop](https://www.docker.com/products/docker-desktop/) ([alternatives](https://code.visualstudio.com/remote/advancedcontainers/docker-options))
- **Start Docker Desktop**
- Install [Visual Studio Code](https://code.visualstudio.com)
- Clone and open this repository
- Install the [Dev Containers](vscode:extension/ms-vscode-remote.remote-containers) extension (you should be prompted for recommended extensions)
- Click the blue 'Reopen in Container' button when prompted (you can also find it in the command palette)

For more detailed steps, check out the [Dev Containers tutorial](https://code.visualstudio.com/docs/devcontainers/tutorial). The instructions after this assume you are running _inside_ the container.

## Building the examples

The container [`Dockerfile`](./.devcontainer/Dockerfile) contains all the tools and extensions necessary to develop LLVM-based tools in C++. To build the examples, use the following commands from VSCode's command palette (Meta+Shift+P):

- CMake: Configure
- CMake: Build
- clangd: Restart language server

## Activating the environment

After building, activate the environment from the Terminal:

```bash
source llvm-env
```

To verify you should be able to run `llvm-config --prefix`:

```bash
/cxx-common/install
```

The `${workspaceFolder}/build` directory will also be added to your `PATH`, so you can easily access your tools from anywhere. To check if the examples built correctly you can run `hello`:

```sh
remill version: e72a100
```
