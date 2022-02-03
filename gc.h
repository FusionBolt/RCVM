#pragma once

#include <set>
#include "rc_object.h"

namespace RCVM
{
    class GC
    {
    public:
        RcObject *stack_obj_alloc(const std::string& klass)
        {
            return new_obj(klass, GC_TAG_STACK);
        }

        RcObject *static_obj_alloc(const std::string& klass)
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
    GC gc;
}