#include <cstdlib>
#include <memory>
#include <string>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

static void CheckModule(const llvm::Module &M, const char *stage) {
  std::string Error;
  llvm::raw_string_ostream RSO(Error);
  bool BrokenDebugInfo = false;
  if (llvm::verifyModule(M, &RSO, &BrokenDebugInfo)) {
    while (!Error.empty() && std::isspace(Error.back())) {
      Error.pop_back();
    }
    llvm::errs() << "[" << stage << "] VERIFY ERROR\n----------\n" << Error << "\n----------\n";
  } else {
    llvm::errs() << "[" << stage << "] VERIFY OK\n\n";
  }
}

int main(int argc, char **argv) {
  // Load module
  llvm::LLVMContext C;
  std::unique_ptr<llvm::Module> M = std::make_unique<llvm::Module>("basics", C);

  CheckModule(*M, "empty");

  // Create the function prototype
  llvm::Type *ReturnTy = llvm::Type::getInt32Ty(C);
  std::vector<llvm::Type *> ParamTyp = {
    llvm::Type::getInt32Ty(C),
    llvm::Type::getInt32Ty(C),
  };
  llvm::FunctionType *FunctionTy = llvm::FunctionType::get(ReturnTy, ParamTyp, false);

  // Create the function
  llvm::Function *MyFunction = llvm::Function::Create(FunctionTy,
    llvm::Function::ExternalLinkage,
    "myFunction",
    *M);

  CheckModule(*M, "function");

  // Create a basic block
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(C, "bb1", MyFunction);
  CheckModule(*M, "bb1");

  // Insert an instruction
  llvm::IRBuilder<> Builder(BB);
  llvm::Value *Arg1 = MyFunction->getArg(0);
  llvm::Value *Leet = llvm::ConstantInt::get(llvm::Type::getInt32Ty(C), 1337);
  llvm::Value *Sum = Builder.CreateAdd(Arg1, Leet);

  // Print the basic block
  BB->print(llvm::outs(), nullptr, true, true);

  // Show that verification fails
  CheckModule(*M, "add");

  // Add the return instruction
  Builder.CreateRet(Sum);
  CheckModule(*M, "ret");

  llvm::outs() << "Final module:\n";
  M->print(llvm::outs(), nullptr, true, true);

  return EXIT_SUCCESS;
}
