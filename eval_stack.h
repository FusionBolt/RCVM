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
                _stack_top(_memory.get()),
                _stack_bottom(_stack_top) {}

        EvalStack(const EvalStack&) = delete;
        EvalStack operator=(const EvalStack&) = delete;

        void push(int value) {
            top_value() = value;
            stack_move(1);
        }

        // stack_top point to address which will be writen
        // need move down before read
        int pop()
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

        int& top_value() {
            return get_value(0);
        }

        int& get_value(int offset) {
            return *get_top_offset(offset);
        }

        void begin_call(size_t argc, size_t locals, size_t ret_addr)
        {
            // 1.set stack base
            auto *base = get_args_begin(argc);
            // 2.alloc local var space
            _stack_top = stack_move(base, static_cast<int>(locals));
            // 3.create new stack frame
            _frame = std::make_shared<StackFrame>(_frame, base, ret_addr);
        }

        size_t end_call()
        {
            auto ret_addr = _frame->ret_addr();
            _stack_top = stack_move(_frame->base(), -1);
            _frame = _frame->prev();
            return ret_addr;
        }

        // positive only
        int get_local(size_t offset)
        {
            return *get_base_offset(static_cast<int>(offset));
        }

        void set_local(size_t offset, int value)
        {
            *get_base_offset(static_cast<int>(offset)) = value;
        }

        bool empty() const { return _frame == nullptr; }

        // todo:const
        std::vector<int> current_data()
        {
            std::vector<int> vars;
            auto count = (_stack_top - _frame->base()) / MoveOffset;
            for (int i = 0; i < count; ++i) {
                vars.push_back(*get_base_offset(i));
            }
            return vars;
        }

    private:
        char *stack_move(char *stack_pos, int offset) const
        {
            return stack_pos + offset * MoveOffset;
        }

        void stack_move(int offset)
        {
            _stack_top = _stack_top + offset * MoveOffset;
        }

        int *get_top_offset(int offset)
        {
            return get_offset_pos(_stack_top, offset);
        }

        int *get_base_offset(int offset)
        {
            return get_offset_pos(_frame->base(), offset);
        }

        int *get_offset_pos(char *base, int offset)
        {
            return reinterpret_cast<int*>(base + offset * MoveOffset);
        }

        char *get_args_begin(size_t argc)
        {
            return _stack_top - argc * MoveOffset;
        }

        std::unique_ptr<char[]> _memory;
        // todo:unique_ptr?
        std::shared_ptr<StackFrame> _frame;
        char *_stack_top;
        char *_stack_bottom;
    };
}