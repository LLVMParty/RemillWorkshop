#pragma once

#include <cstdlib>
#include <memory>
#include <unordered_map>

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Bitcode/BitcodeWriter.h>

inline std::unique_ptr<llvm::Module> LoadModule(llvm::LLVMContext &Context,
  const std::string &Filename) {
  llvm::SMDiagnostic Err;
  auto M = llvm::parseIRFile(Filename, Err, Context);
  if (!M) {
    llvm::errs() << "Failed to parse IR: " << Err.getMessage() << "\n";
    llvm::errs().flush();
    std::exit(EXIT_FAILURE);
  }
  return M;
}

inline void SaveModule(llvm::Module *Module, const std::string &Filename) {
  if (Filename.ends_with(".ll") || Filename.ends_with(".txt")) {
    std::error_code EC;
    llvm::raw_fd_ostream RFO(Filename, EC);
    Module->print(RFO, nullptr, true, true);
  } else if (Filename.ends_with(".bc")) {
    std::error_code EC;
    llvm::ToolOutputFile Out(Filename, EC, llvm::sys::fs::OF_None);
    WriteBitcodeToFile(*Module, Out.os(), true);
    if (EC) {
      llvm::errs() << "Failed to write IR: " << EC.message() << "\n";
      llvm::errs().flush();
      std::exit(EXIT_FAILURE);
    }
    Out.keep();
  } else {
    llvm::errs() << "Unsupported output extension for filename '" << Filename << "'\n";
    llvm::errs().flush();
    std::exit(EXIT_FAILURE);
  }
}

inline const char *ConstantExprClassName(const llvm::ConstantExpr *expr) {
#define HANDLE_INST(num, opcode, Class) {num, "ConstantExpr(" #opcode ")"},
  static std::unordered_map<unsigned, const char *> names = {
#include "llvm/IR/Instruction.def"
  };
  auto itr = names.find(expr->getOpcode());
  return itr == names.end() ? "<unknown>" : itr->second;
}

inline const char *InstructionClassName(const llvm::Instruction *instr) {
#define HANDLE_INST(num, opcode, Class) {num, #Class "(" #opcode ")"},
  static std::unordered_map<unsigned, const char *> names = {
#include "llvm/IR/Instruction.def"
  };
  auto itr = names.find(instr->getOpcode());
  return itr == names.end() ? "<unknown>" : itr->second;
}

inline const char *ValueClassName(const llvm::Value *value) {
  auto id = value->getValueID();
  if (auto instr = dyn_cast<llvm::Instruction>(value)) {
    return InstructionClassName(instr);
  }
  if (auto expr = dyn_cast<llvm::ConstantExpr>(value)) {
    return ConstantExprClassName(expr);
  }
#define HANDLE_VALUE(Name) {llvm::Value::Name##Val, #Name},
  static std::unordered_map<unsigned, const char *> names = {
#include "llvm/IR/Value.def"
  };
  auto itr = names.find(id);
  return itr == names.end() ? "<unknown>" : itr->second;
}
