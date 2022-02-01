#pragma once

#include "symbol_table.hpp"

namespace RCVM {
    struct RcBasic {
        // pointer to class, Constant or string
        std::string klass;
        // flag
        bool mark;
    };

    class RcObject {
        // object info
        RcBasic basic;
        // value
    public:
        bool get_mark() const {
            return basic.mark;
        }

        void make_mark() {
            basic.mark = true;
        }
    };
}