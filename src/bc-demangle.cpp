#include <cstdlib>

#include "utility.hpp"
#include <llvm/Demangle/Demangle.h>

using namespace llvm;

static std::string CustomDemangle(const std::string &Name) {
  auto Demangled = demangle(Name);
  while (true) {
    auto AnonIdx = Demangled.find("(anonymous namespace)::");
    if (AnonIdx == std::string::npos) {
      break;
    }
    Demangled = Demangled.substr(0, AnonIdx) + Demangled.substr(AnonIdx + 23);
  }
  return Demangled;
}

static void ProcessModule(Module &M) {
  for (Function &F : M.functions()) {
    F.setName(CustomDemangle(F.getName().str()));
  }
  for (GlobalVariable &G : M.globals()) {
    G.setName(CustomDemangle(G.getName().str()));
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
