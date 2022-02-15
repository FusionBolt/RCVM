#include <gtest/gtest.h>
#include "helper.h"
#include "rcvm.h"

using namespace RCVM;
using namespace RCVMTest;
class VMTestEnv : public testing::Environment
{
public:
    void SetUp() override {
        register_kernel_table();
        register_foo();
        register_inherit_class();
    }
};

TEST(VMTest, init)
{
    VM vm;
    // todo:add default kernel implement
    vm.set_can_stop();
    vm.run();
    auto &stack = vm.eval_stack();
    ASSERT_EQ(stack.depth(), 1);
    ASSERT_EQ(stack.get_object(0)->klass(), VM::VMGlobalClass);
    ASSERT_EQ(vm.pc(), 0);
    ASSERT_EQ(vm.pc_need_incr(), true);
}

TEST(VMTest, pc)
{
    // call a fun and pc not incr
}

TEST(VMTest, method_search)
{
    VM vm;
    ASSERT_NO_THROW(
            auto method = vm.method_search("Child", "f1");
                    );

    ASSERT_THROW(vm.method_search("Child", "f2"), MethodNotFoundException);

    ASSERT_NO_THROW(
            auto method = vm.method_search("Child", "parent_f1");
            );
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    testing::Environment* env = new VMTestEnv();
    testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}