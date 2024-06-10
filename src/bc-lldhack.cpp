#include <cstdlib>

#include "utility.hpp"

using namespace llvm;

static void ProcessModule(Module &M) {
  auto &context = M.getContext();
  auto mdArg1 = llvm::MDString::get(context, "arg1");
  auto mdArg2 = llvm::MDString::get(context, "arg2");
  llvm::Metadata *mdStrings[] = {mdArg1, mdArg2};
  auto mdNode = llvm::MDNode::get(context, mdStrings);

  M.addModuleFlag(llvm::Module::Warning, "lld.command-line", mdNode);
  M.addModuleFlag(llvm::Module::Warning,
    "lld.working-directory",
    llvm::MDString::get(context, "/workspaces/remill-workshop/exercises/1_llvmir"));
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
