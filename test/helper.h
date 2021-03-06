#pragma once
#include "symbol_table.h"
#include "compiler_defs.h"
#include "rcvm.h"

namespace RCVMTest
{
    void register_kernel_table()
    {
        RCVM::SymbolTable<RCVM::FunInfo> kernel_methods;
        auto &info = kernel_methods.define(RCVM::VM::VMEntryFun, RCVM::FunInfo());
        info.inst_list = { std::make_shared<Return>() };
        RCVM::global_class_table.define(RCVM::VM::VMGlobalClass, RCVM::ClassInfo({}, kernel_methods, {}));
    }

    void register_foo()
    {
        RCVM::global_class_table.define("Foo", RCVM::ClassInfo({"var"}, {}, {}));
    }

    void register_inherit_class()
    {
        RCVM::SymbolTable<RCVM::FunInfo> parent_methods;
        parent_methods.define("parent_f1", RCVM::FunInfo());
        RCVM::SymbolTable<RCVM::FunInfo> child_methods;
        child_methods.define("f1", RCVM::FunInfo());
        RCVM::global_class_table.define("Parent", RCVM::ClassInfo({"parent_v"}, parent_methods, {}));
        RCVM::global_class_table.define("Child", RCVM::ClassInfo({"child_v"}, child_methods, {"Parent"}));
    }

#define MAIN(env_name) \
int main(int argc, char *argv[]) \
{ \
    testing::InitGoogleTest(&argc, argv); \
    testing::Environment* env = new env_name(); \
    testing::AddGlobalTestEnvironment(env); \
    return RUN_ALL_TESTS(); \
}
}