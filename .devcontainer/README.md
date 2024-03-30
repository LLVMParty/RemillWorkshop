# Docker image creation

The base image comes from: https://github.com/mrexodia/cxx-common-cmake. For the devcontainer we always use the upstream tag and overwrite it in this repo.

Build instructions:

```sh
docker buildx build --platform linux/arm64,linux/amd64 -t ghcr.io/llvmparty/remillworkshop:8f93b1cf .
docker push ghcr.io/llvmparty/remillworkshop:8f93b1cf
```
