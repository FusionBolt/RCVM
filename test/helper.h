#pragma once
#include <symbol_table.hpp>

namespace RCVMTest
{
    void register_kernel_table()
    {
        RCVM::global_class_table.define("Kernel", RCVM::ClassInfo());
    }

    void register_foo()
    {
        RCVM::global_class_table.define("Foo", RCVM::ClassInfo());
    }
}