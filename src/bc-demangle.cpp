#include <cstdlib>

#include <llvm/Demangle/Demangle.h>

#include "utility.hpp"

using namespace llvm;

static void ReplaceAll(std::string &str, const std::string &from, const std::string &to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
}

static void ProcessModule(Module &M) {
  for (Function &F : M.functions()) {
    auto demangled = demangle(F.getName().str());
    ReplaceAll(demangled, "(anonymous namespace)::", "");
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
    errs() << x.what() << "\n";
    return EXIT_FAILURE;
  }

  // Save the module
  SaveModule(M.get(), outFile);

  return EXIT_SUCCESS;
}
