#pragma once

#include <set>
#include <rc_object.h>
#include <symbol_table.hpp>

namespace RCVM
{
    class GC
    {
    public:
        GC() = default;

        GC(const GC&) = delete;
        GC operator=(const GC&) = delete;

        RcObject *alloc_stack_obj(const std::string& klass)
        {
            return new_obj(klass, GC_TAG_STACK);
        }

        RcObject *alloc_static_obj(const std::string& klass)
        {
            return new_obj(klass, GC_TAG_STATIC);
        }

        void run()
        {
            mark_phase();
            sweep_phase();
        }

        void mark_phase()
        {
            // visit all stack frame
        }

        void sweep_phase()
        {
            // 1. dfs visit
            // 1.1 obj in stack
            // 1.2 obj in obj member

            // avoid cycle ref

            // 2. mark
        }

        ~GC()
        {

        }
    private:

        RcObject *new_obj(const std::string& klass, Flag flag) {
            if(!global_class_table.contains(klass))
            {
                throw std::runtime_error("Class " + klass + " Not Exist");
            }
            // todo: how to save number and obj?
            auto pointer = new RcObject(klass, flag, global_class_table[klass]._vars.size());
            if(pointer == nullptr)
            {
                throw std::runtime_error("alloc nullptr");
                //run();
            }
            _obj_list.insert(pointer);
            // compute klass size
            return pointer;
        }

        std::set<RcObject *> _obj_list;
    };
    inline GC gc;
}