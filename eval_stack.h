#pragma once
#include "stack_frame.h"
#include "rc_object.h"

class O{
    O() = default;
    O(std::shared_ptr<O> _o) : o(_o) {}
    std::shared_ptr<O> o;
};
class F {
    F() : o(std::shared_ptr<O>(nullptr)) {}

    void f()
    {
        o = std::shared_ptr<O>(o);
    }
    std::shared_ptr<O> o;
};

namespace RCVM {
    class EvalStack {
    public:
        using WordT = long;
        constexpr static size_t MoveOffset = sizeof(WordT);
        // 2MB stack memory
        EvalStack() :
                _memory(std::make_unique<unsigned char[]>(1024 * 2048)),
                _frame(std::make_shared<StackFrame>(nullptr, nullptr, 0, nullptr)),
                _stack_top(_memory.get()),
                _stack_bottom(_stack_top) { }

        EvalStack(const EvalStack&) = delete;
        EvalStack operator=(const EvalStack&) = delete;

        void push(long value) {
            top_value() = value;
            stack_move(1);
        }

        template <typename T>
        void push_pointer(T *p)
        {
            push(reinterpret_cast<uintptr_t>(p));
        }
        // stack_top point to address which will be writen
        // need move down before read
        WordT pop()
        {
            stack_move(-1);
            auto value = top_value();
            return value;
        }

        template<typename Callable>
        void exec(Callable &&f)
        {
            auto new_v = f(pop(), pop());
            push(new_v);
        }

        WordT& top_value() {
            return get_value(0);
        }

        WordT& get_value(int offset) {
            return *get_top_offset(offset);
        }

        uintptr_t& get_top_pointer_value(int offset) {
            return *reinterpret_cast<uintptr_t*>(get_top_offset(offset));
        }

        RcObject *get_this(size_t argc) {
            // neg offset
            auto ptr = get_top_pointer_value(static_cast<int>(-(argc + 1)));
            return reinterpret_cast<RcObject*>(ptr);
        }

        void begin_call(size_t argc, size_t locals, size_t ret_addr, RcObject *this_ptr)
        {
            // 1.set stack base
            auto *base = get_args_begin(argc);
            // 2.alloc local var space
            _stack_top = stack_move(base, static_cast<int>(locals));
            // 3.create new stack frame
            _frame = std::make_shared<StackFrame>(nullptr, base, ret_addr, this_ptr);
        }

        size_t end_call()
        {
            auto ret_addr = _frame->ret_addr();
            _stack_top = stack_move(_frame->base(), -1);
            _frame = _frame->prev();
            return ret_addr;
        }

        // positive only
        WordT get_local(size_t offset)
        {
            return *get_base_offset(static_cast<int>(offset));
        }

        void set_local(size_t offset, WordT value)
        {
            *get_base_offset(static_cast<int>(offset)) = value;
        }

        bool empty() const { return _frame == nullptr; }

        // todo:const
        std::vector<WordT> current_data()
        {
            std::vector<WordT> vars;
            auto count = (_stack_top - _frame->base()) / MoveOffset;
            for (int i = 0; i < count; ++i) {
                vars.push_back(*get_base_offset(i));
            }
            return vars;
        }

    private:
        Pointer stack_move(Pointer stack_pos, int offset) const
        {
            return stack_pos + offset * MoveOffset;
        }

        void stack_move(int offset)
        {
            _stack_top = _stack_top + offset * MoveOffset;
        }

        WordT *get_top_offset(int offset)
        {
            return get_offset_pos(_stack_top, offset);
        }

        WordT *get_base_offset(int offset)
        {
            return get_offset_pos(_frame->base(), offset);
        }

        WordT *get_offset_pos(Pointer base, int offset)
        {
            return reinterpret_cast<WordT*>(base + offset * MoveOffset);
        }

        Pointer get_args_begin(size_t argc)
        {
            return _stack_top - argc * MoveOffset;
        }

        std::unique_ptr<unsigned char[]> _memory;
        // todo:unique_ptr?
        std::shared_ptr<StackFrame> _frame;
        Pointer _stack_top;
        Pointer _stack_bottom;
    };
}