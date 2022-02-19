#include <gtest/gtest.h>
#include "helper.h"
#include "exception.h"
#include <rcvm.h>

using namespace RCVM;
using namespace RCVMTest;
class SymbolTableTestEnv : public testing::Environment
{
public:
    void SetUp() override {
        register_kernel_table();
        register_foo();
        register_inherit_class();

        RCVM::global_class_table.define("SymbolTableChild", RCVM::ClassInfo({"child_v", "parent_v"}, {}, {"Parent"}));
    }
};

TEST(ClassVarsSizeTest, normal)
{
    auto size = class_vars_size("Child");
    ASSERT_EQ(size, 2);
}

TEST(ClassVarSizeTest, redefined_var)
{
    auto size = class_vars_size("SymbolTableChild");
    ASSERT_EQ(size, 2);
}

MAIN(SymbolTableTestEnv)