#include <gtest/gtest.h>
#include <rc_object.h>
#include <gc.h>
#include "helper.h"

using namespace RCVM;
using namespace RCVMTest;
class ObjectTestEnv : public testing::Environment
{
public:
    void SetUp() override {
        RCVM::global_class_table.define("Foo", RCVM::ClassInfo({"v1, v2, v3"}, {}, {}));
    }
};

TEST(field_setter, set_value)
{
    auto *obj = gc.alloc_stack_obj("Foo");
    auto value = 7;
    obj->set_value(0, value);
    auto result = obj->get_number_field(0);
    ASSERT_EQ(value, result);
}

TEST(field_setter, set_pointer)
{
    auto *obj = gc.alloc_stack_obj("Foo");
    auto *new_obj = gc.alloc_stack_obj("Foo");
    obj->set_pointer(0, new_obj);
    auto result = obj->get_ptr_field(0);
    ASSERT_EQ(new_obj, result);
}

TEST(field_setter, mix)
{
    auto *obj = gc.alloc_stack_obj("Foo");
    auto *new_obj = gc.alloc_stack_obj("Foo");
    EvalStack stack;
    stack.push_pointer(new_obj);
    auto obj_addr = stack.pop();
    obj->set_value(0, obj_addr);
    auto result = obj->get_ptr_field(0);
    ASSERT_EQ(new_obj, result);
}

// todo:add test for inherit
MAIN(ObjectTestEnv)