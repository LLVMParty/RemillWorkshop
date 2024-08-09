#include <cstdlib>

#include "utility.hpp"
#include "llvm/IR/AssemblyAnnotationWriter.h"

using namespace llvm;

static const char *ConstantExprClassName(const ConstantExpr *expr) {
#define HANDLE_INST(num, opcode, Class) {num, "ConstantExpr(" #opcode ")"},
  static std::map<unsigned, const char *> names = {
#include "llvm/IR/Instruction.def"
  };
  auto itr = names.find(expr->getOpcode());
  return itr == names.end() ? "<unknown>" : itr->second;
}

static const char *InstructionClassName(const Instruction *instr) {
#define HANDLE_INST(num, opcode, Class) {num, #Class "(" #opcode ")"},
  static std::map<unsigned, const char *> names = {
#include "llvm/IR/Instruction.def"
  };
  auto itr = names.find(instr->getOpcode());
  return itr == names.end() ? "<unknown>" : itr->second;
}

static const char *ValueClassName(const Value *value) {
  auto id = value->getValueID();
  if (auto instr = dyn_cast<Instruction>(value)) {
    return InstructionClassName(instr);
  }
  if (auto expr = dyn_cast<ConstantExpr>(value)) {
    return ConstantExprClassName(expr);
  }
#define HANDLE_VALUE(Name) {Value::Name##Val, #Name},
  static std::map<unsigned, const char *> names = {
#include "llvm/IR/Value.def"
  };
  auto itr = names.find(id);
  return itr == names.end() ? "<unknown>" : itr->second;
}

struct MyAnnotationWriter : AssemblyAnnotationWriter {
  void printConstantExprOps(const ConstantExpr *expr, formatted_raw_ostream &OS, int depth) {
    auto numOperands = expr->getNumOperands();
    for (unsigned opIndex = 0; opIndex < numOperands; opIndex++) {
      if (opIndex > 0) {
        OS << ",\n";
      }
      auto op = expr->getOperand(opIndex);
      OS << "  ; ";
      OS.indent(depth * 4);
      OS << "[" << opIndex << "] ";
      if (auto exprOp = dyn_cast<ConstantExpr>(op)) {
        OS << ConstantExprClassName(exprOp);
        OS << " -> ";
        op->getType()->print(OS, false, true);
        OS << " {\n";
        printConstantExprOps(exprOp, OS, depth + 1);
        OS << "\n  ; ";
        OS.indent(depth * 4);
        OS << "}";
      } else {
        OS << ValueClassName(op);
        OS << " -> ";
        op->getType()->print(OS, false, true);
        OS << " ";
        op->printAsOperand(OS, false);
      }
    }
  }

  void emitInstructionAnnot(const Instruction *I, formatted_raw_ostream &OS) override {
    OS << "  ; ";
    OS << ValueClassName(I);
    OS << " -> ";
    I->getType()->print(OS, false, true);
    auto numOperands = I->getNumOperands();
    if (numOperands > 0) {
      OS << " {\n";
      for (unsigned opIndex = 0; opIndex < numOperands; opIndex++) {
        if (opIndex > 0) {
          OS << ",\n";
        }
        OS << "  ;   [" << opIndex << "] ";
        auto op = I->getOperand(opIndex);
        if (auto Phi = dyn_cast<PHINode>(I)) {
          OS << "(Instruction";
          OS << " -> ";
          op->getType()->print(OS, false, true);
          OS << " ";
          op->printAsOperand(OS, false);

          OS << ", BasicBlock -> label ";
          auto block = Phi->getIncomingBlock(opIndex);
          block->printAsOperand(OS, false);
          OS << ")";
        } else if (visited.contains(op)) {
          OS << "Instruction";
          OS << " -> ";
          op->getType()->print(OS, false, true);
          OS << " ";
          op->printAsOperand(OS, false);
        } else {
          OS << ValueClassName(op);
          OS << " -> ";
          op->getType()->print(OS, false, true);
          OS << " ";
          if (auto CX = dyn_cast<ConstantExpr>(op)) {
            OS << "{\n";
            printConstantExprOps(CX, OS, 2);
            OS << "\n  ;   }";
          } else {
            op->printAsOperand(OS, false);
          }
        }
      }
      OS << "\n  ; }";
    }
    OS << "\n";
    visited.emplace(I);
  }

  void printInfoComment(const Value &V, formatted_raw_ostream &OS) override {
    if (!visited.contains(&V)) {
      OS << " ; ";
      OS << ValueClassName(&V);
    }
  }

private:
  std::set<const Value *> visited;
};

int main(int argc, char **argv) {
  // Parse arguments
  if (argc < 2) {
    printf("Usage: %s in.bc [out.ll]\n", std::filesystem::path(argv[0]).filename().c_str());
    return EXIT_FAILURE;
  }
  auto inFile = argv[1];
  auto outFile = argc > 2 ? argv[2] : nullptr;

  // Load module
  LLVMContext C;
  auto M = LoadModule(C, inFile);

  // Create output file
  auto out = &outs();
  if (outFile != nullptr) {
    std::error_code EC;
    out = new llvm::raw_fd_ostream(outFile, EC);
  }

  // Process module
  MyAnnotationWriter AW;
  M->print(*out, &AW);

  // Save module
  if (outFile != nullptr) {
    delete out;
  }

  return EXIT_SUCCESS;
}
