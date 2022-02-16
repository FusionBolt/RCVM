#include "eval_stack.h"

void RCVM::EvalStack::push(long value) {
    write_pos_value() = value;
    stack_move(1);
}

RCVM::EvalStack::WordT RCVM::EvalStack::pop() {
    stack_move(-1);
    auto value = write_pos_value();
    return value;
}

void RCVM::EvalStack::begin_call(size_t argc, size_t locals, size_t ret_addr,
                                 RCVM::RcObject *this_ptr, const string &f) {
    // 1.set stack base
    auto *base = get_args_begin(argc);
    // 2.alloc local var space
    _stack_top = stack_move(base, static_cast<int>(locals));
    // 3.create new stack frame
    _frame = std::make_shared<StackFrame>(_frame, base, ret_addr, this_ptr, f);
    // 4.increase depth
    ++_depth;
}

size_t RCVM::EvalStack::end_call() {
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

using namespace RCVM;

std::vector<EvalStack::WordT> RCVM::EvalStack::current_data() {
    std::vector<WordT> vars;
    auto count = (_stack_top - _frame->base()) / MoveOffset;
    for (int i = 0; i < count; ++i) {
        vars.push_back(*get_base_offset(i));
    }
    return vars;
}

std::vector<EvalStack::WordT> EvalStack::frame_data(const StackFrame *frame, const Pointer next_base) {
    std::vector<WordT> vars;
    auto count = (next_base - frame->base()) / MoveOffset;
    for (int i = 0; i < count; ++i) {
        vars.push_back(*get_frame_base_offset(frame, i));
    }
    return vars;
}

std::vector<std::vector<EvalStack::WordT>> EvalStack::all_frame_data() {
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
