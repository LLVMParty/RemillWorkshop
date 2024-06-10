#include <cstdlib>

#include "utility.hpp"

using namespace llvm;

static void ProcessModule(Module &M) {
  for (Function &F : M.functions()) {
    outs() << ">> " << F.getName() << " <<\n";
    int loads = 0;
    int phis = 0;
    for (BasicBlock &BB : F) {
        outs() << "  BB: " << BB.getName() << "\n";
      for (Instruction &I : BB) {
        if (auto Load = dyn_cast<LoadInst>(&I)) {
            loads++;
        } else if (auto Phi = dyn_cast<PHINode>(&I)) {
            phis++;
        }
      }
    }
    outs() << "  loads: " << loads << "\n";
    outs() << "  phis: " << phis << "\n";
  }
}

int main(int argc, char **argv) {
  // Parse arguments
  if (argc < 2) {
    printf("Usage: %s in.bc [out.bc]\n", std::filesystem::path(argv[0]).filename().c_str());
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

  return EXIT_SUCCESS;
}
