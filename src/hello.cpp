#include <llvm/Support/raw_ostream.h>
#include <remill/Version/Version.h>

int main() {
  llvm::outs() << "remill version: " << remill::version::GetVersionString() << "\n";
}
