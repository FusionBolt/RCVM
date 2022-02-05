#pragma once

#include <set>
#include <rc_object.h>
#include <symbol_table.hpp>

namespace RCVM
{
    class GC
    {
    public:
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
            // 1. dfs visit
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
            auto pointer = new RcObject(klass, flag);
            if(pointer == nullptr)
            {
                throw std::runtime_error("alloc nullptr");
                //run();
            }
            obj_list.insert(pointer);
            // compute klass size
            return pointer;
        }

        std::set<RcObject *> obj_list;
    };
    inline GC gc;
}