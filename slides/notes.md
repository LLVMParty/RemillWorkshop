## Slides

- https://llvm.org/docs/GetElementPtr.html
- show GEP before/after optimizations
- better phi example
- llvm: risc
- remember to use api-basics.cpp during the presentation

## LLVM Exercises

- Activate the environment per default
- Walk everybody through setting up the build
- Show crash and how to debug it (gdb, callstack)
- Mostly just play around and look at the IR
- Show the concepts

## API Exercises

- StringRef/Twine -> std::string
- Write some tools to print statistics
- Graphviz
- TODO: Verify the module?

## Remill Exercises

- Lift something more complex
- Write the demangler
- Lift some hashing algorithm?
  - Symbolic execution/decompilation?

## Setup

- It takes around 1 hour for 7 people to get everything working (probably doing it in the cloud will be better)
- Docker Desktop needs to be updated
- Windows: git core.autocrlf issues
- Stress to clone using https
- Mac: Permission issues in `~/.docker/buildx/current` (chown fernando:staff)
- Codespaces will most likely be a much better idea for a public workshop
