#include <cstdlib>

#include "utility.hpp"

using namespace llvm;

static void ProcessModule(Module &M) {
  auto RAM = M.getGlobalVariable("RAM");
  if (RAM == nullptr) {
    throw std::runtime_error("Cannot find @RAM variable!");
  }
  std::vector<Instruction *> erase;
  std::vector<User *> users(RAM->user_begin(), RAM->user_end());
  for (const auto &user : users) {
    if (auto GEP = dyn_cast<GetElementPtrInst>(user)) {
      auto V = GEP->getOperand(2);
      IRBuilder<> ir(GEP->getParent());
      ir.SetInsertPoint(GEP);
      auto PtrTy = Type::getInt8PtrTy(M.getContext());
      auto Cast = ir.CreateIntToPtr(V, PtrTy);
      GEP->replaceAllUsesWith(Cast);
      erase.push_back(GEP);
    } else {
      throw std::runtime_error("Unexpected @RAM usage");
    }
  }
  for (auto &e : erase) {
    e->eraseFromParent();
  }
  RAM->eraseFromParent();
  std::vector<Function *> intrinsics;
  for (Function &F : M.functions()) {
    if (F.getName().startswith("__remill_")) {
      intrinsics.push_back(&F);
    }
  }
  for (auto i : intrinsics) {
    i->eraseFromParent();
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