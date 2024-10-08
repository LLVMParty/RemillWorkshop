#include <cstdlib>

#include "utility.hpp"

using namespace llvm;

static void ProcessModule(Module &M) {
  // TODO: do something meaningful here
  for (Function &F : M.functions()) {
    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        if (auto Load = dyn_cast<LoadInst>(&I)) {
        }
      }
    }
  }
}

int main(int argc, char **argv) {
  // Parse arguments
  if (argc < 2) {
    printf("Usage: %s in.bc [out.bc]\n", argv[0]);
    return EXIT_FAILURE;
  }
  auto inFile = argv[1];
  auto outFile = argc > 2 ? argv[2] : nullptr;

  // Load module
  LLVMContext C;
  auto M = LoadModule(C, inFile);

  // Process module
  try {
    ProcessModule(*M);
  } catch (const std::exception &x) {
    outs() << x.what() << "\n";
    return EXIT_FAILURE;
  }

  // Save module
  if (outFile != nullptr) {
    SaveModule(M.get(), outFile);
  }

  outs() << "Verifying module...\n";
  if (verifyModule(*M, &outs())) {
    outs() << "Module verification failed!\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
