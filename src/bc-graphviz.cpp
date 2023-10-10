#include <cstdlib>
#include <fstream>

#include "utility.hpp"

using namespace llvm;

int main(int argc, char **argv) {
  // Parse arguments
  if (argc < 4) {
    printf("Usage: %s in.bc functionName out.dot\n",
      std::filesystem::path(argv[0]).filename().c_str());
    return EXIT_FAILURE;
  }
  auto irFile = argv[1];
  auto functionName = argv[2];
  auto dotFile = argv[3];

  // Load module
  LLVMContext C;
  auto module = LoadModule(C, irFile);

  // Find the function
  auto function = module->getFunction(functionName);
  if (function == nullptr) {
    llvm::errs() << "Could not find function: " << functionName << "\n";
    return EXIT_FAILURE;
  }

  // Create the output file
  std::error_code ec;
  llvm::raw_fd_ostream dot(dotFile, ec);
  if (ec) {
    llvm::errs() << "Failed to create output file\n";
    return EXIT_FAILURE;
  }

  // Generate the graph
  dot << "digraph G {\n";
  for (const auto &block : *function) {
    for (const auto &successor : llvm::successors(&block)) {
      dot << "  \"" << block.getName() << "\" -> \"" << successor->getName() << "\";\n";
    }
  }
  dot << "}\n";

  return EXIT_SUCCESS;
}
