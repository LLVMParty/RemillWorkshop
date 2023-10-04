#include <cstdlib>

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Bitcode/BitcodeWriter.h>

using namespace llvm;

static void ProcessModule(Module &M) {
  // TODO: do something meaningful here
  for (Function &F : M.functions()) {
    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        if (auto Load = dyn_cast<LoadInst>(&I)) {
        }
      }
    }
  }
}

int main(int argc, char **argv) {
  if (argc < 3) {
    puts("Usage: process-module in.bc out.bc");
    return EXIT_FAILURE;
  }
  auto inFile = argv[1];
  auto outFile = argv[2];

  // Load module
  LLVMContext C;
  SMDiagnostic Err;
  auto M = parseIRFile(inFile, Err, C);
  if (!M) {
    outs() << "Failed to parse IR: " << Err.getMessage() << "\n";
    return EXIT_FAILURE;
  }

  // Process module
  try {
    ProcessModule(*M);
  } catch (const std::exception &x) {
    outs() << x.what() << "\n";
    return EXIT_FAILURE;
  }

  // Write module
  std::error_code EC;
  ToolOutputFile Out(outFile, EC, sys::fs::OF_None);
  WriteBitcodeToFile(*M, Out.os(), true);
  if (EC) {
    outs() << "Failed to write IR: " << EC.message() << "\n";
    return EXIT_FAILURE;
  }
  Out.keep();

  return EXIT_SUCCESS;
}
