#include <cstdlib>

#include "utility.hpp"

#include <llvm/Demangle/Demangle.h>

using namespace llvm;

static void ProcessModule(Module &M) {
  for (Function &F : M.functions()) {
    auto demangled = demangle(F.getName().str());
    outs() << F.getName() << " -> " << demangled << "\n";
    F.setName(demangled);
  }
}

int main(int argc, char **argv) {
  // Parse arguments
  if (argc < 3) {
    printf("Usage: %s in.bc out.bc\n", std::filesystem::path(argv[0]).filename().c_str());
    return EXIT_FAILURE;
  }
  auto inFile = argv[1];
  auto outFile = argv[2];

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
  SaveModule(M.get(), outFile);

  return EXIT_SUCCESS;
}
