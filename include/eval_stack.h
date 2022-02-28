#pragma once

#include "stack_frame.h"
#include "rc_object.h"

namespace RCVM {
    enum class BinaryOp {
        Add,
        Sub,
        Mul,
        Div,
        Mod,
        EQ,
        LT,
        GT
    };

    class EvalStack {
    public:
        using WordT = long;
        constexpr static size_t MoveOffset = sizeof(WordT);

        // 2MB stack memory
        EvalStack() :
                _memory(std::make_unique<unsigned char[]>(1024 * 2048)),
                _frame(std::make_shared<StackFrame>(nullptr, _memory.get(), 0, nullptr, InitMethodName)),
                _stack_top(_memory.get()),
                _stack_bottom(_stack_top) {}

        EvalStack(const EvalStack &) = delete;

        EvalStack operator=(const EvalStack &) = delete;

        void push(long value);

        template<typename T>
        void push_pointer(T *p) {
            push(reinterpret_cast<uintptr_t>(p));
        }

        // stack_top point to address which will be writen
        // need move down before read
        WordT pop();

        void exec(BinaryOp op) {
#define PUSH(_opname, _op) \
           case BinaryOp::_opname: \
              push(v1 _op v2);     \
              break;

            // LT GT, FILO
            auto v2 = pop();
            auto v1 = pop();
            switch (op) {
                PUSH(Add, +)
                PUSH(Sub, -)
                PUSH(Mul, *)
                PUSH(Div, /)
                PUSH(Mod, %)
                PUSH(EQ, ==)
                PUSH(LT, <)
                PUSH(GT, >)
            }
#undef PUSH
        }

        template<typename Callable>
        void exec(Callable &&f) {
            auto new_v = f(pop(), pop());
            push(new_v);
        }

        WordT &write_pos_value() {
            return get_value(0);
        }

        WordT &get_value(int offset) {
            return *get_top_offset_ptr(offset);
        }

        uintptr_t &get_top_pointer_value(int offset) {
            // 1. get pointer which point to stack data
            // 2. reinterpret_cast from uintptr_t to long, because of the opposite of push
            // 3. get value for pointer , this value is obj address
            return *reinterpret_cast<uintptr_t *>(get_top_offset_ptr(offset));
        }

        RcObject *get_object(size_t argc) {
            // neg offset
            auto ptr = get_top_pointer_value(-static_cast<int>(argc + 1));
            return reinterpret_cast<RcObject *>(ptr);
        }

        [[nodiscard]] RcObject *this_ptr() const {
            return _frame->this_ptr();
        }

        [[nodiscard]] std::string current_method() const {
            return _frame->method();
        }

        void begin_call(size_t argc, size_t locals, size_t ret_addr, RcObject *this_ptr, const std::string &f);

        size_t end_call();

        size_t depth() const { return _depth; }

        // positive only
        WordT get_local(size_t offset) {
            return *get_base_offset(static_cast<int>(offset));
        }

        void set_local(size_t offset, WordT value) {
            *get_base_offset(static_cast<int>(offset)) = value;
        }

        // todo:const
        std::vector<WordT> current_data();

        std::vector<WordT> frame_data(const StackFrame *frame, const Pointer next_base);

        std::vector<std::vector<WordT>> all_frame_data();

        StackFrame *current_frame() const {
            return _frame.get();
        }

        ConstPointer bottom() const {
            return _stack_bottom;
        }

        ConstPointer top() const {
            return _stack_top;
        }

    private:
        Pointer stack_move(Pointer stack_pos, int offset) const {
            return stack_pos + offset * MoveOffset;
        }

        void stack_move(int offset) {
            _stack_top = _stack_top + offset * MoveOffset;
        }

        WordT *get_top_offset_ptr(int offset) {
            return get_offset_pos(_stack_top, offset);
        }

        WordT *get_base_offset(int offset) {
            return get_offset_pos(_frame->base(), offset);
        }

        WordT *get_frame_base_offset(const StackFrame *frame, int offset) {
            return get_offset_pos(frame->base(), offset);
        }

        WordT *get_offset_pos(Pointer base, int offset) {
            return reinterpret_cast<WordT *>(base + offset * MoveOffset);
        }

        Pointer get_args_begin(size_t argc) {
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