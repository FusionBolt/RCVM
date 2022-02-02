#pragma once

#include <set>
#include "rc_object.h"

namespace RCVM
{
    class GC
    {
    public:
        RcObject *new_obj([[maybe_unused]] const std::string& klass) {
            auto pointer = new RcObject();
            if(pointer == nullptr)
            {
                run();
            }
            obj_list.insert(pointer);
            // compute klass size
            return pointer;
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
        std::set<RcObject *> obj_list;
    };
    GC gc;
}