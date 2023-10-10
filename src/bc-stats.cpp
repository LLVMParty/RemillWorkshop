#include <cstdlib>

#include "utility.hpp"

using namespace llvm;

static void ProcessModule(Module &M) {
  for (Function &F : M.functions()) {
    int loads = 0;
    int stores = 0;
    int calls = 0;
    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        if (auto Load = dyn_cast<LoadInst>(&I)) {
          loads++;
        } else if (auto Store = dyn_cast<StoreInst>(&I)) {
          stores++;
        } else if (auto Call = dyn_cast<CallInst>(&I)) {
          calls++;
        }
      }
    }
    llvm::outs() << "Function: " << F.getName() << "\n";
    llvm::outs() << "   load: " << loads << "\n";
    llvm::outs() << "  store: " << stores << "\n";
    llvm::outs() << "   call: " << calls << "\n";
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
