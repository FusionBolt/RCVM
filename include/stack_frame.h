#pragma once
#include <memory>
#include "compiler_defs.h"
#include "rc_object.h"

namespace RCVM {
    class StackFrame {
    public:
        StackFrame(std::shared_ptr<StackFrame> prev, Pointer base, size_t ret_addr, RcObject *this_ptr, std::string method)
                : _prev(prev), _base(base), _ret_addr(ret_addr), _this_ptr(this_ptr), _method(std::move(method)) {}

        StackFrame(const StackFrame &other) = default;

        StackFrame(StackFrame &&other) = default;

        StackFrame &operator=(const StackFrame &other) = default;

        StackFrame &operator=(StackFrame &&other) = default;

        std::shared_ptr<StackFrame> prev() { return _prev; }

        Pointer base() const { return _base; }

        size_t ret_addr() const { return _ret_addr; }

        RcObject *this_ptr() const { return _this_ptr; }

        std::string method() const { return _method; }

    private:
        std::shared_ptr<StackFrame> _prev;
        Pointer _base;
        size_t _ret_addr;
        RcObject *_this_ptr;
        std::string _method;
    };
}