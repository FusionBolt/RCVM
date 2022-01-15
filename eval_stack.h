#pragma once
#include "stack_frame.h"

namespace RCVM {
    class EvalStack {
    public:
        constexpr static size_t MoveOffset = sizeof(int);
        // 2MB stack memory
        EvalStack() :
                _memory(std::make_unique<char[]>(1024 * 2048)),
                _frame(nullptr),
                _stack_top(_memory.get()) {
        }

        EvalStack(const EvalStack&) = delete;
        EvalStack operator=(const EvalStack&) = delete;

        void push(int value) {
            top_value() = value;
            _stack_top += MoveOffset;
        }

        void pop([[maybe_unused]] int target) {
            // target addr set value top_value()
            _stack_top -= MoveOffset;
        }

        template<typename Callable>
        void exec(Callable &&f)
        {
            auto new_v = f(get_value(0), get_value(4));
            _stack_top += MoveOffset * 2;
            push(new_v);
        }

        int& top_value() {
            return get_value(0);
        }

        int& get_value(int offset) {
            return *reinterpret_cast<int*>(_stack_top - offset);
        }

        void begin_call(size_t ret_addr)
        {
            _frame = std::make_shared<StackFrame>(_frame, _stack_top, ret_addr);
        }

        size_t end_call()
        {
            auto ret_addr = _frame->ret_addr();
            _stack_top = _frame->base() - 1;
            _frame = _frame->prev();
            return ret_addr;
        }

        bool empty() const { return _frame == nullptr; }
    private:
        std::unique_ptr<char[]> _memory;
        // todo:unique_ptr?
        std::shared_ptr<StackFrame> _frame;
        char *_stack_top;
    };
}