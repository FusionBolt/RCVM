#pragma once
#include <memory>

class StackFrame
{
public:
    StackFrame(std::shared_ptr<StackFrame> prev, char *base, size_t ret_addr)
    : _prev(prev), _base(base), _ret_addr(ret_addr) {}

    StackFrame(const StackFrame& other) = default;
    StackFrame(StackFrame&& other) = default;
    StackFrame& operator=(const StackFrame& other) = default;
    StackFrame& operator=(StackFrame&& other) = default;

    std::shared_ptr<StackFrame> prev() { return _prev; }

    char *base() { return _base; }

    size_t ret_addr() { return _ret_addr; }

private:
    std::shared_ptr<StackFrame> _prev;
    char *_base;
    size_t _ret_addr;
};