#include <iostream>
#include <stdint.h>

#include "llvm/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Assembly/PrintModulePass.h"

#include "llvm/Analysis/Verifier.h"
#include "llvm/Module.h"
#include "llvm/Target/TargetData.h"
#include "llvm/DerivedTypes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/Interpreter.h"

#include "llvm/LLVMContext.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ManagedStatic.h"

#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetSubtargetInfo.h"
#include "llvm/Target/TargetLowering.h"

using namespace std;
using namespace llvm;

namespace llvm {
    class BasicBlock;
    class ExecutionEngine;
    class Function;
    class FunctionPassManager;
    class Module;
    class PointerType;
    class StructType;
    class Value;
    class LLVMContext;
}

int main(void) {
    LLVMContext *llvmContext = new LLVMContext();
    Module *module = new Module("module", *llvmContext);
    IRBuilder <> irBuilder(*llvmContext);

    //int a = 10;
    module->getOrInsertGlobal("a", irBuilder.getInt32Ty());
    GlobalVariable *a = module->getNamedGlobal("a");
    a->setInitializer(irBuilder.getInt32(10));

    // int *p = &a;
    GlobalVariable *p = static_cast<GlobalVariable *>( // The second code writing method
        module->getOrInsertGlobal("p", PointerType::getInt32PtrTy(*llvmContext))
    );
    p->setInitializer(a);

    // int *p2 = p;
    GlobalVariable *p2 = new GlobalVariable(*module, PointerType::getInt32PtrTy(*llvmContext),
                                         true, GlobalValue::CommonLinkage, 0, "p2", 0, false, 0);
    p2->setInitializer(p);

    // double d[2] = {1.1, 2.2, 3.3};
    ArrayType *arrayType = ArrayType::get(irBuilder.getDoubleTy(), 2);
    module->getOrInsertGlobal("d", arrayType);
    GlobalVariable *d = module->getNamedGlobal("d");

    vector <Constant *> array_elems;
    Constant *const_tmp;

    // d[0]
    const_tmp = ConstantFP::get(irBuilder.getDoubleTy(), 1.1);
    array_elems.push_back(const_tmp);

    // d[1]
    const_tmp = ConstantFP::get(irBuilder.getDoubleTy(), 2.2);
    array_elems.push_back(const_tmp);

    // d[2]
    const_tmp = ConstantFP::get(irBuilder.getDoubleTy(), 3.3);
    array_elems.push_back(const_tmp);

    d->setInitializer(ConstantArray::get(arrayType, array_elems));

    // struct s{ int a, float b, double d, int *p};
    StructType *structType = StructType::create(*llvmContext, "s");
    vector <Type * > struct_elems;
    struct_elems.push_back(irBuilder.getInt32Ty());
    struct_elems.push_back(irBuilder.getFloatTy());
    struct_elems.push_back(irBuilder.getDoubleTy());
    struct_elems.push_back(PointerType::getInt32PtrTy(*llvmContext));
    structType->setBody(struct_elems, true);

    // sa = {1, 1.1, 2.2, };
    module->getOrInsertGlobal("sa", structType);
    vector <Constant *> struct_elems_init;
    GlobalVariable *sa = module->getNamedGlobal("sa");
    struct_elems_init.push_back(irBuilder.getInt32(1));
    struct_elems_init.push_back(ConstantFP::get(irBuilder.getDoubleTy(), 1.1));
    struct_elems_init.push_back(ConstantFP::get(irBuilder.getDoubleTy(), 2.2));
    struct_elems_init.push_back(ConstantPointerNull::get(PointerType::getInt32PtrTy(*llvmContext)));
    sa->setInitializer(ConstantStruct::get(structType, struct_elems_init));

    module->dump();  //print IR

    llvm_shutdown();

    return 0;
}


