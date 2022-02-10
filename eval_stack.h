#pragma once
#include "stack_frame.h"
#include "rc_object.h"

namespace RCVM {
    class EvalStack {
    public:
        using WordT = long;
        constexpr static size_t MoveOffset = sizeof(WordT);
        // 2MB stack memory
        EvalStack() :
                _memory(std::make_unique<unsigned char[]>(1024 * 2048)),
                _frame(std::make_shared<StackFrame>(nullptr, _memory.get(), 0, nullptr)),
                _stack_top(_memory.get()),
                _stack_bottom(_stack_top) { }

        EvalStack(const EvalStack&) = delete;
        EvalStack operator=(const EvalStack&) = delete;

        void push(long value) {
            write_pos_value() = value;
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
            auto value = write_pos_value();
            return value;
        }

        template<typename Callable>
        void exec(Callable &&f)
        {
            auto new_v = f(pop(), pop());
            push(new_v);
        }

        WordT& write_pos_value() {
            return get_value(0);
        }

        WordT& get_value(int offset) {
            return *get_top_offset(offset);
        }

        uintptr_t& get_top_pointer_value(int offset) {
            return *reinterpret_cast<uintptr_t*>(get_top_offset(offset));
        }

        RcObject *get_object(size_t argc) {
            // neg offset
            auto ptr = get_top_pointer_value(static_cast<int>(-(argc + 1)));
            return reinterpret_cast<RcObject*>(ptr);
        }

        RcObject *this_ptr() const {
            return _frame->this_ptr();
        }

        void begin_call(size_t argc, size_t locals, size_t ret_addr)
        {
            // 1. get new this_ptr
            auto this_ptr = get_object(argc);
            if(this_ptr == nullptr)
            {
                throw std::runtime_error("ThisPtr is nullptr");
            }
            // 2.set stack base
            auto *base = get_args_begin(argc);
            // 3.alloc local var space
            _stack_top = stack_move(base, static_cast<int>(locals));
            // 4.create new stack frame
            _frame = std::make_shared<StackFrame>(_frame, base, ret_addr, this_ptr);
            // 5.increase depth
            ++_depth;
        }

        size_t end_call()
        {
            // todo: test end call, ret value and frame base
            // 1. get ret addr
            auto ret_addr = _frame->ret_addr();
            // 2. stack back
            _stack_top = _frame->base();
            // 3. frame back
            _frame = _frame->prev();
            // 4.decrease depth
            --_depth;
            return ret_addr;
        }

        size_t depth() const { return _depth; }
        // positive only
        WordT get_local(size_t offset)
        {
            return *get_base_offset(static_cast<int>(offset));
        }

        void set_local(size_t offset, WordT value)
        {
            *get_base_offset(static_cast<int>(offset)) = value;
        }

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

        std::vector<WordT> frame_data(const StackFrame *frame, const Pointer next_base)
        {
            std::vector<WordT> vars;
            auto count = (next_base - frame->base()) / MoveOffset;
            for (int i = 0; i < count; ++i) {
                vars.push_back(*get_frame_base_offset(frame, i));
            }
            return vars;
        }

        std::vector<std::vector<WordT>> all_frame_data()
        {
            auto data = std::vector<std::vector<WordT>>();
            auto frame = current_frame();
            Pointer next_base = _stack_top;
            while(frame != nullptr)
            {
                data.push_back(frame_data(frame, next_base));
                next_base = frame->base();
                frame = frame->prev().get();
            }
            std::reverse(data.begin(), data.end());
            return data;
        }


        StackFrame* current_frame() const {
            return _frame.get();
        }

        ConstPointer bottom() const {
            return _stack_bottom;
        }

        ConstPointer top() const {
            return _stack_top;
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

        WordT *get_frame_base_offset(const StackFrame *frame, int offset)
        {
            return get_offset_pos(frame->base(), offset);
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
        size_t _depth = 0;
    };
}