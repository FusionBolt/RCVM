#pragma once

#include <utility>

#include "symbol_table.h"

namespace RCVM {
    using Flag = unsigned char;

    constexpr auto GC_TAG_NONE = 0b0;
    constexpr auto GC_TAG_MARK = 0b1;
    constexpr auto GC_TAG_STACK = 0b10;
    constexpr auto GC_TAG_STATIC = 0b11;

    struct RcBasic {
        RcBasic(std::string klass, Flag flag) : _klass(std::move(klass)), _flag(flag) {}
        // pointer to class, Constant or string
        std::string _klass;
        // flag
        Flag _flag;

        const string &klass() const {
            return _klass;
        }

        Flag flag() const {
            return _flag;
        }

        bool has_flag(Flag flag) const {
            return _flag & flag;
        }

        void add_flag(Flag flag) {
            _flag |= flag;
        }
    };

    class RcObject {
        // object info
        RcBasic basic;
        // value
    public:
        std::vector<RcObject*> fields;

        RcObject(std::string klass, Flag flag, size_t vars)
            : basic(std::move(klass), flag), fields(vars, nullptr) {}
        bool get_mark() const {
            return has_flag(GC_TAG_MARK);
        }

        bool has_flag(Flag flag) const {
            return basic.has_flag(flag);
        }

        Flag flag() const {
            return basic.flag();
        }

        void make_mark() {
            basic.add_flag(GC_TAG_MARK);
        }

        std::string klass() const {
            return basic.klass();
        }

        void set_pointer(int index, RcObject *value) {
            fields[index] = value;
        }

        void set_value(int index, int64_t value) {
            fields[index] = reinterpret_cast<RcObject*>(value);
        }

        int64_t get_number_field(int index) const {
            return reinterpret_cast<int64_t>(fields[index]);
        }

        RcObject *get_ptr_field(int index) const {
            return fields[index];
        }
    };
}