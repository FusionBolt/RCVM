#include <gtest/gtest.h>
#include <eval_stack.h>
#include <gc.h>
#include "helper.h"

using namespace RCVM;
using namespace RCVMTest;
class EvalStackTestEnv : public testing::Environment
{
public:
    void SetUp() override {
        register_kernel_table();
        register_foo();
    }
};

TEST(EvalStackTest, push_normal_value)
{
    EvalStack stack;
    auto push_v = 5;
    stack.push(push_v);
    auto pop_v = stack.pop();
    ASSERT_EQ(push_v, pop_v);
}

TEST(EvalStackTest, push_pointer)
{
    EvalStack stack;
    auto obj = gc.alloc_stack_obj("Foo");
    stack.push_pointer(obj);
    auto this_ptr = stack.get_object(0);
    ASSERT_EQ(this_ptr, obj);
}

TEST(EvalStackTest, exec)
{
    EvalStack stack;
    stack.push(3);
    stack.push(4);
    stack.exec([](auto a, auto b){ return a + b;});
    auto result = stack.pop();
    ASSERT_EQ(result, 7);
}


TEST(EvalStackTest, fun_call)
{
    EvalStack stack;
    auto obj = gc.alloc_stack_obj("Foo");
    stack.push_pointer(obj);
    stack.begin_call(3, 5, 0);
    auto frame = stack.get_current_frame();
    stack.end_call();
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    testing::Environment* env = new EvalStackTestEnv();
    testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}