#include <cstdlib>

#include "utility.hpp"

using namespace llvm;

static void ProcessModule(Module &M) {
  // TODO: do something meaningful here
  for (Function &F : M.functions()) {
    outs() << "digraph \"" << F.getName() << "\" {\n";
    for (BasicBlock &BB : F) {
#if 0
      auto Terminator = BB.getTerminator();
      if (auto Ret = dyn_cast<ReturnInst>(Terminator)) {
      } else if (auto Br = dyn_cast<BranchInst>(Terminator)) {
        if (Br->isUnconditional()) {
          auto Dest = Br->getSuccessor(0);
          outs() << "unconditional: " << Dest->getName() << "\n";
        } else {
            outs() << "  \"" << BB.getName() << "\" -> \"" << TrueDest->getName()
          auto TrueDest = Br->getSuccessor(0);
          auto FalseDest = Br->getSuccessor(1);
          outs() << "true: " << TrueDest->getName() << "\n";
          outs() << "false: " << FalseDest->getName() << "\n";
        }
      } else {
        outs() << "  [TODO] ";
        Terminator->print(outs(), true);
        outs() << "\n";
      }
#endif
#if 1
      auto hasSucc = false;
      for (auto succ : successors(&BB)) {
        hasSucc = true;
        outs() << "  \"" << BB.getName() << "\" -> \"" << succ->getName() << "\"\n";
      }
      if (!hasSucc) {
        outs() << "  \"" << BB.getName() << "\"\n";
      }
#endif
    }
    outs() << "}\n";
    // break;
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
