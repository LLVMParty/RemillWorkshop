# remill-workshop

## Dev Container

This repository uses a [`devcontainer.json`](./.devcontainer/devcontainer.json) file to allow you to quickly get started:

- Install [Docker](https://code.visualstudio.com/remote/advancedcontainers/docker-options) and [Visual Studio Code](https://code.visualstudio.com)
- Open this repository
- Click 'Open in Dev Container' when prompted

For more information, check out the [Dev Containers tutorial
](https://code.visualstudio.com/docs/devcontainers/tutorial).

## Activating the environment

Once you are inside the Dev Container, you can access the installed tools from `/cxx-common/install`. To add them to your path, run:

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
