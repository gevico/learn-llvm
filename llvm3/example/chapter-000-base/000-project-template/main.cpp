#include <iostream>

#include "llvm/Module.h"
#include "llvm/LLVMContext.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/ManagedStatic.h"

using namespace std;
using namespace llvm;

namespace llvm {
    class Module;
    class LLVMContext;
}

int main(void) {
    LLVMContext *llvmContext = new LLVMContext();
    Module *module = new Module("module", *llvmContext);
    IRBuilder <> irBuilder(*llvmContext);

    llvm_shutdown();

    return 0;
}


