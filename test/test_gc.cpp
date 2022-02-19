#include <gtest/gtest.h>
#include <gc.h>
#include "helper.h"

using namespace RCVM;
using namespace RCVMTest;
class GCTestEnv : public testing::Environment
{
public:
    void SetUp() override {
        register_kernel_table();
        register_foo();
    }
};

TEST(GCTest, alloc_class_not_exist)
{
    EXPECT_ANY_THROW({
        try
        {
            gc.alloc_stack_obj("NotExist");
        }
        catch( const std::runtime_error& e)
        {
            EXPECT_STREQ("Class NotExist Not Exist", e.what());
            throw;
        }
    });
}

TEST(GCTest, normal_alloc)
{
    auto *obj = gc.alloc_stack_obj("Foo");
    EXPECT_EQ(obj->flag(), GC_TAG_STACK);
    EXPECT_EQ(obj->klass(), "Foo");
}

TEST(GCTest, alloc_static)
{
    auto *obj = gc.alloc_static_obj("Foo");
    obj->has_flag(GC_TAG_STATIC);
}

TEST(GCTest, alloc_stack)
{
    auto *obj = gc.alloc_stack_obj("Foo");
    obj->has_flag(GC_TAG_STACK);
}

MAIN(GCTestEnv)