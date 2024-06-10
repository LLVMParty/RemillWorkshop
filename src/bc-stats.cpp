#include <cstdlib>

#include "utility.hpp"

using namespace llvm;

static void ProcessModule(Module &M) {
  // TODO: do something meaningful here
  for (Function &F : M.functions()) {
    int loads = 0;
    int stores = 0;
    int calls = 0;
    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        if (isa<LoadInst>(&I)) {
          loads++;
        } else if (isa<StoreInst>(&I)) {
          stores++;
        } else if (isa<CallInst>(&I)) {
          calls++;
        }
      }
    }
    outs() << "[" << F.getName() << "]\n";
    outs() << "   load: " << loads << "\n";
    outs() << "  store: " << stores << "\n";
    outs() << "   call: " << calls << "\n";
  }
}

int main(int argc, char **argv) {
  // Parse arguments
  if (argc < 2) {
    printf("Usage: %s in.bc\n", std::filesystem::path(argv[0]).filename().c_str());
    return EXIT_FAILURE;
  }
  auto inFile = argv[1];

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

  return EXIT_SUCCESS;
}
