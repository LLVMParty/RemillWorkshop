/*
 * Copyright (c) 2019 Trail of Bits, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <remill/Arch/Arch.h>
#include <remill/Arch/Instruction.h>
#include <remill/Arch/Name.h>
#include <remill/BC/ABI.h>
#include <remill/BC/IntrinsicTable.h>
#include <remill/BC/Lifter.h>
#include <remill/BC/Optimizer.h>
#include <remill/BC/Util.h>
#include <remill/BC/Version.h>
#include <remill/OS/OS.h>
#include <remill/Version/Version.h>
#include <LIEF/LIEF.hpp>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <system_error>

DEFINE_string(os,
  REMILL_OS,
  "Operating system name of the code being "
  "translated. Valid OSes: linux, macos, windows, solaris.");
DEFINE_string(arch,
  REMILL_ARCH,
  "Architecture of the code being translated. "
  "Valid architectures: x86, amd64 (with or without "
  "`_avx` or `_avx512` appended), aarch64, aarch32");

DEFINE_uint64(address,
  -1,
  "Address at which we should assume the bytes are "
  "located in virtual memory.");

DEFINE_uint64(entry_address,
  -1,
  "Address of instruction that should be "
  "considered the entrypoint of this code. "
  "Defaults to the value of --address.");

DEFINE_string(bytes, "", "Hex-encoded byte string to lift.");

DEFINE_string(binary, "", "Path to a binary to lift from (ELF/Mach-O/PE).");

DEFINE_string(ir_pre_out,
  "",
  "Path to the file where the LLVM IR (before optimization) should be saved");

DEFINE_string(ir_out, "", "Path to file where the LLVM IR should be saved.");
DEFINE_string(bc_out,
  "",
  "Path to file where the LLVM bitcode should be "
  "saved.");

DEFINE_string(call_inputs, "", "Comma-separated list of registers to treat as inputs.");
DEFINE_string(call_output, "", "Return register.");

DEFINE_bool(globalregs, false, "Whether registers are converted to global variables.");

using Memory = std::map<uint64_t, uint8_t>;

// Unhexlify the data passed to `--bytes`, and fill in `memory` with each
// such byte.
static Memory UnhexlifyInputBytes(uint64_t addr_mask) {
  Memory memory;

  for (size_t i = 0; i < FLAGS_bytes.size(); i += 2) {
    char nibbles[] = {FLAGS_bytes[i], FLAGS_bytes[i + 1], '\0'};
    char *parsed_to = nullptr;
    auto byte_val = strtol(nibbles, &parsed_to, 16);

    if (parsed_to != &(nibbles[2])) {
      std::cerr << "Invalid hex byte value '" << nibbles << "' specified in --bytes." << std::endl;
      exit(EXIT_FAILURE);
    }

    auto byte_addr = FLAGS_address + (i / 2);
    auto masked_addr = byte_addr & addr_mask;

    // Make sure that if a really big number is specified for `--address`,
    // that we don't accidentally wrap around and start filling out low
    // byte addresses.
    if (masked_addr < byte_addr) {
      std::cerr << "Too many bytes specified to --bytes, would result "
                << "in a 32-bit overflow.";
      exit(EXIT_FAILURE);

    } else if (masked_addr < FLAGS_address) {
      std::cerr << "Too many bytes specified to --bytes, would result "
                << "in a 64-bit overflow.";
      exit(EXIT_FAILURE);
    }

    memory[byte_addr] = static_cast<uint8_t>(byte_val);
  }

  return memory;
}

class SimpleTraceManager : public remill::TraceManager {
public:
  Memory &memory;
  LIEF::Binary *binary = nullptr;
  std::unordered_map<uint64_t, llvm::Function *> traces;

  virtual ~SimpleTraceManager(void) = default;

  explicit SimpleTraceManager(Memory &memory_, LIEF::Binary *binary_)
    : memory(memory_)
    , binary(binary_) {
  }

  // Called when we have lifted, i.e. defined the contents, of a new trace.
  // The derived class is expected to do something useful with this.
  void SetLiftedTraceDefinition(uint64_t addr, llvm::Function *lifted_func) override {
    traces[addr] = lifted_func;
  }

  // Get a declaration for a lifted trace. The idea here is that a derived
  // class might have additional global info available to them that lets
  // them declare traces ahead of time. In order to distinguish between
  // stuff we've lifted, and stuff we haven't lifted, we allow the lifter
  // to access "defined" vs. "declared" traces.
  //
  // NOTE: This is permitted to return a function from an arbitrary module.
  llvm::Function *GetLiftedTraceDeclaration(uint64_t addr) override {
    auto trace_it = traces.find(addr);
    if (trace_it != traces.end()) {
      return trace_it->second;
    } else {
      return nullptr;
    }
  }

  // Get a definition for a lifted trace.
  //
  // NOTE: This is permitted to return a function from an arbitrary module.
  llvm::Function *GetLiftedTraceDefinition(uint64_t addr) override {
    return GetLiftedTraceDeclaration(addr);
  }

  // Try to read an executable byte of memory. Returns `true` of the byte
  // at address `addr` is executable and readable, and updates the byte
  // pointed to by `byte` with the read value.
  bool TryReadExecutableByte(uint64_t addr, uint8_t *byte) override {
    if (binary != nullptr) {
      if (!isExecutableSection(addr)) {
        return false;
      }
      std::vector<uint8_t> content = binary->get_content_from_virtual_address(addr, 1);
      if (content.empty()) {
        std::cerr << "Warning: reached end of section?" << std::endl;
        return false;
      }
      if (byte != nullptr) {
        *byte = content[0];
      }
      return true;
    } else {
      auto byte_it = memory.find(addr);
      if (byte_it != memory.end()) {
        if (byte != nullptr) {
          *byte = byte_it->second;
        }
        return true;
      } else {
        return false;
      }
    }
  }

private:
  bool isExecutableSection(const uint64_t addr) const {
    auto format = binary->format();
    switch (format) {
    case LIEF::EXE_FORMATS::FORMAT_ELF: {
      auto *elf = dynamic_cast<const LIEF::ELF::Binary *>(binary);
      auto *section = elf->section_from_virtual_address(addr);
      if (!section)
        return false;
      return section->has(LIEF::ELF::ELF_SECTION_FLAGS::SHF_EXECINSTR);
    } break;
    case LIEF::EXE_FORMATS::FORMAT_PE: {
      uint64_t address = addr - binary->imagebase();
      auto *pe = dynamic_cast<const LIEF::PE::Binary *>(binary);
      auto *section = pe->section_from_rva(address);
      if (!section)
        return false;
      return section->has_characteristic(LIEF::PE::SECTION_CHARACTERISTICS::IMAGE_SCN_MEM_EXECUTE);
    } break;
    case LIEF::EXE_FORMATS::FORMAT_MACHO: {
      auto *macho = dynamic_cast<const LIEF::MachO::Binary *>(binary);
      auto *section = macho->section_from_virtual_address(addr);
      if (!section)
        return false;
      return section->has(LIEF::MachO::MACHO_SECTION_FLAGS::S_ATTR_PURE_INSTRUCTIONS);
    } break;
    default: {
      throw std::runtime_error("Unhandled LIEF format");
    } break;
    }
    return false;
  }
};

// Looks for calls to a function like `__remill_function_return`, and
// replace its state pointer with a null pointer so that the state
// pointer never escapes.
static void MuteStateEscape(llvm::Module *module, const char *func_name) {
  auto func = module->getFunction(func_name);
  if (!func) {
    return;
  }

  for (auto user : func->users()) {
    if (auto call_inst = llvm::dyn_cast<llvm::CallInst>(user)) {
      auto arg_op = call_inst->getArgOperand(remill::kStatePointerArgNum);
      call_inst->setArgOperand(remill::kStatePointerArgNum,
        llvm::UndefValue::get(arg_op->getType()));
    }
  }
}

static void SetVersion(void) {
  std::stringstream ss;
  auto vs = remill::version::GetVersionString();
  if (0 == vs.size()) {
    vs = "unknown";
  }
  ss << vs << "\n";
  if (!remill::version::HasVersionData()) {
    ss << "No extended version information found!\n";
  } else {
    ss << "Commit Hash: " << remill::version::GetCommitHash() << "\n";
    ss << "Commit Date: " << remill::version::GetCommitDate() << "\n";
    ss << "Last commit by: " << remill::version::GetAuthorName() << " ["
       << remill::version::GetAuthorEmail() << "]\n";
    ss << "Commit Subject: [" << remill::version::GetCommitSubject() << "]\n";
    ss << "\n";
    if (remill::version::HasUncommittedChanges()) {
      ss << "Uncommitted changes were present during build.\n";
    } else {
      ss << "All changes were committed prior to building.\n";
    }
  }
  google::SetVersionString(ss.str());
}

int main(int argc, char *argv[]) {
  SetVersion();
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  std::unique_ptr<LIEF::Binary> binary;

  if (!FLAGS_binary.empty()) {
    // Parse the binary
    binary = LIEF::Parser::parse(FLAGS_binary);
    if (!binary) {
      std::cerr << "Failed to parse binary (unsupported format?)." << std::endl;
      return EXIT_FAILURE;
    }
    // Extract the OS
    if (FLAGS_os.empty()) {
      switch (binary->format()) {
      case LIEF::FORMAT_PE:
        FLAGS_os = "windows";
        break;
      case LIEF::FORMAT_ELF:
        FLAGS_os = "linux";
        break;
      case LIEF::FORMAT_MACHO:
        FLAGS_os = "macos";
        break;
      default:
        std::cerr << "Could not determine OS from binary" << std::endl;
        return EXIT_FAILURE;
      }
    }
    // Extract the architecture
    if (FLAGS_arch.empty()) {
      if (binary->header().endianness() == LIEF::ENDIANNESS::ENDIAN_BIG) {
        std::cerr << "Big endian not supported" << std::endl;
        return EXIT_FAILURE;
      }
      switch (binary->header().architecture()) {
      case LIEF::ARCH_X86:
        if (binary->header().is_64()) {
          FLAGS_arch = "amd64";
        } else {
          FLAGS_arch = "x86";
        }
        break;
      case LIEF::ARCH_ARM:
        FLAGS_arch = "aarch32";
        break;
      case LIEF::ARCH_ARM64:
        FLAGS_arch = "aarch64";
        break;
      default:
        std::cerr << "Unsupported architecture: " << binary->header().architecture() << std::endl;
        return EXIT_FAILURE;
      }
    }
    // Entry point
    if (FLAGS_address == (uint64_t)-1) {
      FLAGS_address = binary->entrypoint();
      std::cerr << "Address not specified, defaulting to entry point: 0x" << std::hex
                << FLAGS_address << std::endl;
    }
  } else if (FLAGS_bytes.empty()) {
    std::cerr << "Please specify a sequence of hex bytes to -bytes." << std::endl;
    return EXIT_FAILURE;
  } else if (FLAGS_bytes.size() % 2) {
    std::cerr << "Please specify an even number of nibbles to --bytes." << std::endl;
    return EXIT_FAILURE;
  }

  if (FLAGS_address == (uint64_t)-1) {
    FLAGS_address = 0;
  }

  if (FLAGS_entry_address == (uint64_t)-1) {
    FLAGS_entry_address = FLAGS_address;
  }

  // Make sure `--address` and `--entry_address` are in-bounds for the target
  // architecture's address size.
  llvm::LLVMContext context;
  auto arch = remill::Arch::Get(context,
    FLAGS_os,
    FLAGS_arch); // TODO: what happens with invalid arguments?
  const uint64_t addr_mask = ~0ULL >> (64UL - arch->address_size);
  if (FLAGS_address != (FLAGS_address & addr_mask)) {
    std::cerr << "Value " << std::hex << FLAGS_address
              << " passed to --address does not fit into 32-bits. Did mean"
              << " to specify a 64-bit architecture to --arch?" << std::endl;
    return EXIT_FAILURE;
  }

  if (FLAGS_entry_address != (FLAGS_entry_address & addr_mask)) {
    std::cerr << "Value " << std::hex << FLAGS_entry_address
              << " passed to --entry_address does not fit into 32-bits. Did mean"
              << " to specify a 64-bit architecture to --arch?" << std::endl;
    return EXIT_FAILURE;
  }

  std::unique_ptr<llvm::Module> module(remill::LoadArchSemantics(arch.get()));

  const auto mem_ptr_type = arch->MemoryPointerType();
  Memory memory = UnhexlifyInputBytes(addr_mask);
  SimpleTraceManager manager(memory, binary.get());
  if (!manager.TryReadExecutableByte(FLAGS_entry_address, nullptr)) {
    std::cerr << "No executable code at address 0x" << std::hex << FLAGS_entry_address << std::endl;
    return EXIT_FAILURE;
  }
  remill::IntrinsicTable intrinsics(module.get());

  auto inst_lifter = arch->DefaultLifter(intrinsics);

  remill::TraceLifter trace_lifter(arch.get(), manager);

  // Lift all discoverable traces starting from `--entry_address` into
  // `module`.
  trace_lifter.Lift(FLAGS_entry_address);

  // Dump the pre-optimization IR
  if (!FLAGS_ir_pre_out.empty()) {
    if (!remill::StoreModuleIRToFile(module.get(), FLAGS_ir_pre_out, true)) {
      LOG(ERROR) << "Could not save LLVM IR to " << FLAGS_ir_pre_out;
    }
  }

  // Optimize the module, but with a particular focus on only the functions
  // that we actually lifted.
  remill::OptimizationGuide guide = {};
  remill::OptimizeModule(arch, module, manager.traces, guide);

  // Create a new module in which we will move all the lifted functions. Prepare
  // the module for code of this architecture, i.e. set the data layout, triple,
  // etc.
  llvm::Module dest_module("lifted_code", context);
  arch->PrepareModuleDataLayout(&dest_module);

  llvm::Function *entry_trace = nullptr;
  const auto make_slice = !FLAGS_call_inputs.empty() || !FLAGS_call_output.empty();

  // Move the lifted code into a new module. This module will be much smaller
  // because it won't be bogged down with all of the semantics definitions.
  // This is a good JITing strategy: optimize the lifted code in the semantics
  // module, move it to a new module, instrument it there, then JIT compile it.
  for (auto &lifted_entry : manager.traces) {
    if (lifted_entry.first == FLAGS_entry_address) {
      entry_trace = lifted_entry.second;
    }
    remill::MoveFunctionIntoModule(lifted_entry.second, &dest_module);

    // If we are providing a prototype, then we'll be re-optimizing the new
    // module, and we want everything to get inlined.
    if (make_slice) {
      lifted_entry.second->setLinkage(llvm::GlobalValue::InternalLinkage);
      lifted_entry.second->removeFnAttr(llvm::Attribute::NoInline);
      lifted_entry.second->addFnAttr(llvm::Attribute::InlineHint);
      lifted_entry.second->addFnAttr(llvm::Attribute::AlwaysInline);
    }
  }

  if (FLAGS_globalregs) {
    entry_trace->print(llvm::errs(), nullptr, true, true);
    auto state_ptr = entry_trace->getArg(0);
    for (auto &basicBlock : *entry_trace) {
      for (auto &instruction : basicBlock) {
        if (auto gep = llvm::dyn_cast<llvm::GetElementPtrInst>(&instruction)) {
          auto base = gep->getOperand(0);
          if (base == state_ptr) {
            llvm::errs() << "register? " << *gep << "\n";
            arch->ForEachRegister([](const remill::Register *reg) {
              std::string s;
              llvm::raw_string_ostream rso(s);
              for (const auto &index : reg->gep_index_list) {
                rso << *index << " ";
              }
              llvm::errs() << "  " << reg->name << ":" << s << "\n";
            });
          } else {
            llvm::errs() << "unrelated? " << *gep << "\n  " << *base << "\n";
          }
        }
      }
    }
    // llvm::errs() << "entry_trace:\n" << entry_trace << "\n";
    /*arch->ForEachRegister([](const remill::Register *reg) {
      reg->gep_offset;
      reg->gep_type_at_offset;
    });*/
  }

  // We have a prototype, so go create a function that will call our entrypoint.
  if (make_slice) {
    CHECK_NOTNULL(entry_trace);

    llvm::SmallVector<llvm::StringRef, 4> input_reg_names;
    llvm::StringRef output_reg_name = FLAGS_call_output;
    llvm::StringRef(FLAGS_call_inputs).split(input_reg_names, ',', -1, false /* KeepEmpty */);

    CHECK(!(input_reg_names.empty() && output_reg_name.empty()))
      << "Empty lists passed to both --call_inputs and --call_output";

    // Use the registers to build a function prototype.
    llvm::SmallVector<llvm::Type *, 8> arg_types;

    for (auto &reg_name : input_reg_names) {
      const auto reg = arch->RegisterByName(reg_name.str());
      CHECK(reg != nullptr) << "Invalid register name '" << reg_name.str()
                            << "' used in input slice list '" << FLAGS_call_inputs << "'";

      arg_types.push_back(reg->type);
    }

    // Outputs are "returned" by pointer through arguments.
    const auto reg = arch->RegisterByName(output_reg_name);
    CHECK(reg != nullptr) << "Invalid register name '" << output_reg_name.str() << "'";

    const auto state_type = arch->StateStructType();
    const auto func_type = llvm::FunctionType::get(reg->type, arg_types, false);
    const auto func = llvm::Function::Create(func_type,
      llvm::GlobalValue::ExternalLinkage,
      "call_" + entry_trace->getName(),
      &dest_module);

    // Store all of the function arguments (corresponding with specific registers)
    // into the stack-allocated `State` structure.
    auto entry = llvm::BasicBlock::Create(context, "", func);
    llvm::IRBuilder<> ir(entry);

    const auto state_ptr = ir.CreateAlloca(state_type);

    const remill::Register *pc_reg = arch->RegisterByName(arch->ProgramCounterRegisterName());

    CHECK(pc_reg != nullptr) << "Could not find the register in the state structure "
                             << "associated with the program counter.";

    // Store the program counter into the state.
    const auto pc_reg_ptr = pc_reg->AddressOf(state_ptr, entry);
    const auto trace_pc = llvm::ConstantInt::get(pc_reg->type, FLAGS_entry_address, false);
    ir.SetInsertPoint(entry);
    ir.CreateStore(trace_pc, pc_reg_ptr);

    auto args_it = func->arg_begin();
    for (auto &reg_name : input_reg_names) {
      const auto reg = arch->RegisterByName(reg_name.str());
      auto &arg = *args_it++;
      arg.setName(reg_name);
      CHECK_EQ(arg.getType(), reg->type);
      auto reg_ptr = reg->AddressOf(state_ptr, entry);
      ir.SetInsertPoint(entry);
      ir.CreateStore(&arg, reg_ptr);
    }

    llvm::Value *mem_ptr = llvm::UndefValue::get(mem_ptr_type);

    llvm::Value *trace_args[remill::kNumBlockArgs] = {};
    trace_args[remill::kStatePointerArgNum] = state_ptr;
    trace_args[remill::kMemoryPointerArgNum] = mem_ptr;
    trace_args[remill::kPCArgNum] = llvm::ConstantInt::get(
      llvm::IntegerType::get(context, arch->address_size),
      FLAGS_entry_address,
      false);

    mem_ptr = ir.CreateCall(entry_trace, trace_args);

    // Read and return the output register
    const auto out_reg = arch->RegisterByName(output_reg_name);
    auto out_reg_ptr = out_reg->AddressOf(state_ptr, entry);
    ir.CreateRet(ir.CreateLoad(out_reg->type, out_reg_ptr));

    // We want the stack-allocated `State` to be subject to scalarization
    // and mem2reg, but to "encourage" that, we need to prevent the
    // `alloca`d `State` from escaping.
    MuteStateEscape(&dest_module, "__remill_error");
    MuteStateEscape(&dest_module, "__remill_function_call");
    MuteStateEscape(&dest_module, "__remill_function_return");
    MuteStateEscape(&dest_module, "__remill_jump");
    MuteStateEscape(&dest_module, "__remill_missing_block");

    guide.slp_vectorize = true;
    guide.loop_vectorize = true;

    auto check = remill::VerifyModuleMsg(&dest_module);
    if (check) {
      llvm::errs() << "Verification error: " << *check;
      CHECK(false);
    }
    remill::OptimizeBareModule(&dest_module, guide);
  }

  int ret = EXIT_SUCCESS;

  if (!FLAGS_ir_out.empty()) {
    if (!remill::StoreModuleIRToFile(&dest_module, FLAGS_ir_out, true)) {
      LOG(ERROR) << "Could not save LLVM IR to " << FLAGS_ir_out;
      ret = EXIT_FAILURE;
    }
  }
  if (!FLAGS_bc_out.empty()) {
    if (!remill::StoreModuleToFile(&dest_module, FLAGS_bc_out, true)) {
      LOG(ERROR) << "Could not save LLVM bitcode to " << FLAGS_bc_out;
      ret = EXIT_FAILURE;
    }
  }

  return ret;
}
